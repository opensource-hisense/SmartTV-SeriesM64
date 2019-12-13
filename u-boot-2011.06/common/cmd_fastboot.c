#include <config.h>
#include <common.h>
#include <errno.h>
#include <malloc.h>
#include <command.h>
#include <net.h>

#define U32 unsigned int
#define FASTBOOT_VERSION       "0.4"
#define FASTBOOT_RESPONSE_LEN  65    //64 defined bytes+'\0'
#define kDefaultPort           5554
#define CMD_BUF                128

static unsigned short kProtocolVersion=1;


//// fastboot udp packet header
#define kHeaderSize 4
// header fields
typedef enum {
    kIndexId = 0,
    kIndexFlags = 1,
    kIndexSeqH = 2,
    kIndexSeqL = 3,
} Index;
// packet type id
typedef enum {
    kIdError = 0,
    kIdDeviceQuery = 1,
    kIdInitialization = 2,
    kIdFastboot = 3
} Id;
// continuation flag
typedef enum {
    kFlagNone = 0,
    kFlagContinuation = 1
} Flag;


//// fastboot command status
// status type
typedef enum {
    kStatusError = 0,
    kStatusInfo  = 1,
    kStatusOkay  = 2,
    kStatusData  = 3
} StatusCode;
// status structure
typedef struct fb_status_t {
   StatusCode status_code;
   char       message[FASTBOOT_RESPONSE_LEN];
} fb_status_t;
// current status
static fb_status_t fb_status;


//// download status
static int fb_download_flag = 0;
typedef struct fb_download_t {
    U32 total;
    U32 received;
} fb_download_t;
// download status
static fb_download_t fb_download;
// for download progress
static int pkt_count = 0;


//// ip & port
static IPaddr_t fb_remote_ip;
static int      fb_remote_port;
static int      fb_udp_our_port = kDefaultPort;


// buffer for sending response back
char fb_packet[FASTBOOT_RESPONSE_LEN];

// expected next packet sequence
unsigned short fb_expected_seq = 0;

// RAM address to hold the downloaded image
U32 download_addr = 0x20200000;

// for reboot* command
static int fb_need_reset = 0;

// utility to check if s2 starts with s1
static int strcmp_l1(const char *s1, const char *s2)
{
    if (!s1 || !s2)
        return -1;
    return strncmp(s1, s2, strlen(s1));
}

// utility to generate response based on fb_status to generate the response
void fb_gen_resp(char *response)
{
    switch (fb_status.status_code) {
        case kStatusError:
        strncpy(response, "FAIL\0", 5);
        break;
        case kStatusInfo:
        strncpy(response, "INFO\0", 5);
        break;
        case kStatusData:
        strncpy(response, "DATA\0", 5);
        break;
        default:
        strncpy(response, "OKAY\0", 5);
        break;
    }
    strncat(response, fb_status.message, FASTBOOT_RESPONSE_LEN - 4 - 1);
}

// reboot command handler
static void cb_reboot(char *buffer, unsigned buffer_len)
{
    fb_need_reset = 1;
}

// reboot-bootloader command handler
static void cb_reboot_bootloader(char *buffer, unsigned buffer_len)
{
    run_command("setenv reboot-bootloader 1", 0);
    run_command("saveenv", 0);
    fb_need_reset = 1;
}

// getvar command handler
static void cb_getvar(char *buffer, unsigned buffer_len)
{
    // clear the status
    fb_status.status_code = kStatusOkay;
    memset(fb_status.message, 0, FASTBOOT_RESPONSE_LEN);

    if (buffer_len)
    {
        if (!strcmp_l1("version", buffer))
        {
            strcpy(fb_status.message, FASTBOOT_VERSION);
        }
        else if (!strcmp_l1("bootloader-version", buffer))
        {
            // U_BOOT_VERSION undefined
            //strcpy(fb_status.message, U_BOOT_VERSION);
        }
        else
        {
            fb_status.status_code = kStatusError;
            strcpy(fb_status.message, "Variable not implemented");
        }
    }
    else
    {
        fb_status.status_code = kStatusError;
        strcpy(fb_status.message, "Variable not specified");
    }
}

static void cb_download_start(char *buffer, unsigned buffer_len)
{
    // simple_strtoul takes the last valid chars, not the first
    U32 download_size = simple_strtoul(buffer, 0, 16);

    printf("Starting download of %d bytes\n", download_size);

    // init download progress count
    pkt_count = 0;

    if (0 == download_size)
    {
        fb_status.status_code = kStatusError;
        strcpy(fb_status.message, "data invalid size");
//   } else if (download_size > CONFIG_FASTBOOT_BUF_SIZE) {
//      download_size = 0;
//      fb_status.status_code = kStatusError;
//      strcpy(fb_status.message, "data too large");
    }
    else
    {
        fb_status.status_code = kStatusData;
        sprintf(fb_status.message, "%08x", download_size);
        fb_download_flag = 1;
        fb_download.total = download_size;
        fb_download.received = 0;
    }
}

static void cb_download_cont(char *buffer, unsigned buffer_len)
{
    if (pkt_count == 8000) // start a new line every 8000 packets
    {
        printf("\n");
        pkt_count = 0;
    }
    if (!(pkt_count%100)) printf("#"); // print a # every 100 packets
    pkt_count++;

    memcpy((char*)download_addr+fb_download.received, buffer, buffer_len);
    fb_download.received += buffer_len;
}

static void cb_download_end(char *buffer, unsigned buffer_len)
{
    // use download_addr to keep the downloaded data
    memcpy((char*)download_addr+fb_download.received, buffer, buffer_len);
    fb_download.received += buffer_len;
    if (fb_download.received == fb_download.total)
    {
        fb_status.status_code = kStatusOkay;
        memset(fb_status.message, 0, FASTBOOT_RESPONSE_LEN);
    }
    else
    {
        fb_status.status_code = kStatusError;
        strcpy(fb_status.message, "download incomplete");
    }
    fb_download_flag = 0;  // clear download session flag
    printf("\nEnd download, %d of %d bytes\n", fb_download.received, fb_download.total);
}

extern char *get_script_next_line(char **line_buf_ptr);

int util_run_script(char* buffer, unsigned buffer_len)
{
    char* script_buf = 0;
    char* next_line = 0;
    int BeAbort = 1;
    char* tmp = 0;

    // Set commmand abort or not when cmd error.
    tmp = getenv ("CmdAbort");
    if (tmp != NULL)
    {
        BeAbort = (int)simple_strtol(tmp, NULL, 10);
        printf("BeAbort=%d\n",BeAbort);
    }

    script_buf = buffer;
    script_buf[buffer_len]='\n'; script_buf[buffer_len+1]='%';
    while ((next_line = get_script_next_line(&script_buf)) != 0)
    {
        printf("\n>> %s \n", next_line);
        if(-1 == run_command(next_line, 0))
        {
            printf("command error!\n");
            if(BeAbort == 1)
            {
               return -1;
            }
        }
    }
    return 0;
}


static void cb_flash(char *buffer, unsigned buffer_len)
{
    char tmpCmd[128];
    memset(tmpCmd, 0, 128);
    static char last_part_name[CMD_BUF]={0};

    //printf("buffer=%s\n",buffer);
    //printf("last_part_name=%s\n",last_part_name);
    if (!buffer_len)
    {
        fb_status.status_code = kStatusError;
        strcpy(fb_status.message, "missing partition name");
        return;
    }

    if (!strcmp_l1("dl_addr", buffer))
    {
        printf("current downloader_add=0x%08x\n",download_addr);
        download_addr = simple_strtoul((char*)download_addr, NULL, 16);
        printf("set downloader_add=0x%08x\n",download_addr);
        return;
    }


    if (!strcmp_l1("secure_info_boot", buffer))
    {
        sprintf(tmpCmd, "store_secure_info bootSign 0x%08x", download_addr);
        printf("\n%s\n", tmpCmd);
        run_command(tmpCmd, 0);
        return;
    }

    if (!strcmp_l1("secure_info_recovery", buffer))
    {
        sprintf(tmpCmd, "store_secure_info recoverySign 0x%08x", download_addr);
        printf("\n%s\n", tmpCmd);
        run_command(tmpCmd, 0);
        return;
    }

    if (!strcmp_l1("secure_info_tee", buffer))
    {
        sprintf(tmpCmd, "store_secure_info teeSign 0x%08x", download_addr);
        printf("\n%s\n", tmpCmd);
        run_command(tmpCmd, 0);
        return;
    }

    if (!strcmp_l1("nuttx_config", buffer))
    {
        sprintf(tmpCmd, "store_nuttx_config NuttxConfig 0x%08x", download_addr);
        printf("\n%s\n", tmpCmd);
        run_command(tmpCmd, 0);
        return;
    }

    if (!strcmp_l1("rom_emmc_boot", buffer))
    {
        sprintf(tmpCmd, "mmc write.boot 1 0x%08x 0 0x%08x", download_addr, fb_download.received);
        printf("\n%s\n", tmpCmd);
        run_command(tmpCmd, 0);
        return;
    }

    if (!strcmp_l1("mscript", buffer))
    {
        util_run_script((char*)download_addr, fb_download.received);
        return;
    }

    // flash Image
    if (getenv("flash_lzo")!=NULL)
    {
        if (strcmp(buffer,last_part_name)!=0)
        {
            sprintf(tmpCmd, "mmc unlzo 0x%08x 0x%08x %s 1", download_addr, fb_download.received, buffer);
        }
        else
        {
            sprintf(tmpCmd, "mmc unlzo.cont 0x%08x 0x%08x %s 1", download_addr, fb_download.received, buffer);
        }
    }
    else
    {
        sprintf(tmpCmd, "mmc write.p 0x%08x %s 0x%08x 1", download_addr, buffer, fb_download.received);
    }

    if (strlen(tmpCmd))
    {
        printf("\n%s\n", tmpCmd);
        run_command(tmpCmd, 0);
    }
    else
    {
        fb_status.status_code = kStatusError;
        strcpy(fb_status.message, "Unknown partition");
    }

    memset(last_part_name,0,sizeof(last_part_name));
    strcpy(last_part_name, buffer);
}

static void cb_erase(char* buffer, unsigned buffer_len)
{
    char tmpCmd[128];

    memset(tmpCmd, 0, 128);
    sprintf(tmpCmd, "mmc erase.p %s", buffer);
    run_command(tmpCmd, 0);
}

struct cmd_dispatch_info {
    char *cmd;
    void (*cb)(char* buffer, unsigned buffer_len);
};

// download command queries:  "getvar:partition-type:boot" and
//                             getvar:max-download-size
// need to implement this two
static const struct cmd_dispatch_info cmd_dispatch_info[] =
{
    {
        .cmd = "reboot-bootloader",
        .cb = cb_reboot_bootloader,
    }, {
        .cmd = "reboot",
        .cb = cb_reboot,
    }, {
        .cmd = "getvar:",
        .cb = cb_getvar,
    }, {
        .cmd = "download:",
        .cb = cb_download_start,
    }, {
        .cmd = "flash:",
        .cb = cb_flash,
    }, {
        .cmd = "erase:",
        .cb = cb_erase,
    },
};

static void fb_handler_command(char* buffer, unsigned buffer_len)
{
    char cmdbuf[1024];   // assuming max command length
    void (*func_cb)(char* buffer, unsigned buffer_len) = NULL;

    fb_status.status_code = kStatusOkay;
    memset(fb_status.message, 0, FASTBOOT_RESPONSE_LEN);

    if (buffer_len >= 1024)
    {
        fb_status.status_code = kStatusError;
        printf("command too long: %d\n", buffer_len);
        strcpy(fb_status.message, ("command too long"));
        return;
    }

    // put the command in the cmdbuf and add a '\0'
    strncpy(cmdbuf, buffer, buffer_len);
    cmdbuf[buffer_len] = 0;
    printf("\ncommand: %s, length: %d\n", cmdbuf, buffer_len);

    int i;
    for (i = 0; i < ARRAY_SIZE(cmd_dispatch_info); i++)
    {
        if (!strcmp_l1(cmd_dispatch_info[i].cmd, cmdbuf))
        {
            func_cb = cmd_dispatch_info[i].cb;
            break;
        }
    }

    if (!func_cb)
    {
        fb_status.status_code = kStatusError;
        printf("unknown command: %s", cmdbuf);
        strcpy(fb_status.message, ("unknown command"));
    }
    else
    {
        int cmdLen = strlen(cmd_dispatch_info[i].cmd);
        func_cb(cmdbuf+cmdLen, buffer_len-cmdLen);
    }
}


//************************ UDP layer ************************

// Extracts a big-endian uint16_t from a byte array.
unsigned short extract_uint16(const uchar* bytes)
{
    return ((unsigned short)(bytes[0]) << 8) | bytes[1];
}

// Put a big-endian uint16_t to a byte array.
void put_uint16(uchar* bytes, unsigned short value)
{
    bytes[0] = value >> 8;
    bytes[1] = value;
}

void header_set(uchar* header, uchar id, unsigned short sequence, Flag flag)
{
    header[kIndexId] = id;
    header[kIndexFlags] = flag;
    header[kIndexSeqH] = sequence >> 8;
    header[kIndexSeqL] = sequence;
}

void fb_send(char* buffer, unsigned buffer_len)
{
    memcpy((void*)(NetTxPacket + NetEthHdrSize() + IP_HDR_SIZE), buffer, buffer_len);

    // have to use the remote port that we get from udp handler, it is changing
    NetSendUDPPacket(NetServerEther, fb_remote_ip, fb_remote_port,
                    fb_udp_our_port, buffer_len);
}

// send an ACK, for all fastboot packet, an ACK is needed
static void fb_udp_ack(uchar *pkt, unsigned len)
{
    memcpy(fb_packet, pkt, kHeaderSize);
    // need to clear continuation flag
    fb_packet[kIndexFlags] = kFlagNone;
    fb_send(fb_packet, kHeaderSize);
}

// send an error packet
static void fb_udp_error(void)
{
    header_set((uchar*)fb_packet, 0, 0, 0);
    strcpy(fb_packet+kHeaderSize, "packet Id is invalid");
    fb_send(fb_packet, kHeaderSize+strlen(fb_packet+kHeaderSize));
}

static void fb_udp_handler(uchar *pkt, unsigned dest, IPaddr_t sip, unsigned src,
                           unsigned len)
{
    // only handle the packet for us (fastboot)
    if (dest == fb_udp_our_port)
    {
        // need to keep the remote ip and port, needed when sending data back
        fb_remote_ip = sip;
        fb_remote_port = src;

        // incompleted packet
        if (len < kHeaderSize)
            return;

        // read the packet header
        Id             pktId   = pkt[kIndexId];
        Flag           pktFlag = pkt[kIndexFlags];
        unsigned short pktSeq  = extract_uint16(pkt+2);

        // clear the packet for response
        memset(fb_packet, 0, FASTBOOT_RESPONSE_LEN);
        switch(pktId) {
            case kIdDeviceQuery:
            memcpy(fb_packet, pkt, kHeaderSize);
            put_uint16((uchar*)(fb_packet+kHeaderSize), fb_expected_seq);
            fb_send(fb_packet, kHeaderSize+2);
            break;

            case kIdInitialization:
            // seq # is not correct, ignore
            if (pktSeq != fb_expected_seq)
            {
               return;
            }

            if (len < kHeaderSize + 4)
            {
                fb_udp_error();
            }
            else
            {
                unsigned short host_version = extract_uint16(pkt+kHeaderSize);
                unsigned short host_max_packet_len = extract_uint16(pkt+kHeaderSize+2);

                printf("\n**************************************\n");
                printf("host version: %d, host max packet: %d\n",
                      host_version, host_max_packet_len);

                // PKTSIZE doesn't seem work, 1400 seems a good number
                //if (host_max_packet_len > (PKTSIZE - NetEthHdrSize() - IP_HDR_SIZE)) {
                //   host_max_packet_len = PKTSIZE - NetEthHdrSize() - IP_HDR_SIZE;
                //}
                if (host_max_packet_len > 1400)
                {
                    host_max_packet_len = 1400;
                }

                fb_expected_seq++;
                memcpy(fb_packet, pkt, kHeaderSize);
                put_uint16((uchar*)(fb_packet+kHeaderSize), kProtocolVersion);
                put_uint16((uchar*)(fb_packet+kHeaderSize+2), host_max_packet_len);
                fb_send(fb_packet, kHeaderSize+4);
            }
            break;

            case kIdFastboot:
            fb_expected_seq++;
            if (len == kHeaderSize) // empty packet for get status
            {
                memcpy(fb_packet, pkt, kHeaderSize);
                fb_gen_resp(fb_packet+kHeaderSize);
                printf("response: %s\n", fb_packet+kHeaderSize);
                fb_send(fb_packet, kHeaderSize+strlen(fb_packet+kHeaderSize));

                // need to wait for the status packet before resetting
                if (fb_need_reset)
                {
                    mdelay(500);
                    run_command("reset", 0);
                }
            }
            else
            {
                // send ack first
                fb_udp_ack(pkt, len);

                if (fb_download_flag)  // in a download session
                {
                    if (pktFlag)
                        cb_download_cont((char*)(pkt+kHeaderSize), len-kHeaderSize);
                    else
                        cb_download_end((char*)(pkt+kHeaderSize), len-kHeaderSize);
                }
                else  // other fastboot commands
                {
                    fb_handler_command((char*)(pkt+kHeaderSize), len-kHeaderSize);
                }
            }
            break;

            case kIdError:  // fall through
            default:
            // send back a error mesage
            fb_udp_error();
            break;
        }
    }
}

static void fb_udp_init(void)
{
    fb_status.status_code = kStatusOkay;
    memset(fb_status.message, 0, FASTBOOT_RESPONSE_LEN);
    fb_download.total = 0;
    fb_download.received = 0;
    memset(fb_packet, 0, FASTBOOT_RESPONSE_LEN);
}

void fb_udp_start(void)
{
    char sIPStr[32];

    ip_to_string(NetOurIP, sIPStr);
    printf("Using %s device\n", eth_get_name());
    printf("Listening for fastboot transfer on %s\n", sIPStr);

    fb_udp_init();

    NetSetHandler(fb_udp_handler);
}
