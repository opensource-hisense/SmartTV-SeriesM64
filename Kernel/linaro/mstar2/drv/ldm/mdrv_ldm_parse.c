///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//
// @file   mdrv_ldm_parse.c
// @brief  LDM  Driver Interface
// @author MStar Semiconductor Inc.
//////////////////////////////////////////////////////////////////////////////////////////////////

//=============================================================================
// Include Files
//=============================================================================
#if (defined (CONFIG_HAS_LD) || defined (CONFIG_HAS_LD_MODULE))
#include <linux/types.h>
#include <asm/uaccess.h>
#include <linux/fs.h>
#include <asm/string.h>
#include "mdrv_mstypes.h"
#include <linux/ctype.h>
#include <linux/slab.h>

#include "mdrv_ldm_io.h"
#include "mdrv_ldm_common.h"
#include "mdrv_ldm_interface.h"
#include "mdrv_ldm_init.h"
#include "mdrv_ldm_parse.h"


//=============================================================================
// Local Defines
//=============================================================================

#define LDM_BUF_SIZE         4000  // number of LDM.ini member length
#define LDM_BIN_CHECK_POSITION 4    //according bin file

#define SIZEARRAY(a)    (sizeof(a) / sizeof((a)[0]))

#define PQ_GET_INT(str, value)\
    n = MDrv_LD_GetInteger(str, value);\
    LD_INFO("%s = %ld \n",str,n);\
    stDrvLdPQInfo##str = n;

#define PQ_GET_BOOL(str, value)\
    n = MDrv_LD_GetBoolean(str, value);\
    LD_INFO("%s = %ld \n",str,n);\
    stDrvLdPQInfo##str = n;

#define assert(e)
#define _DRVLDPARSE_C_

#if !defined LINE_BUFFERSIZE
#define LINE_BUFFERSIZE  256
#endif



//--------------------------------------------------------------------------------------------------
//  functions declaration
//--------------------------------------------------------------------------------------------------
//static int _mod_ldm_open (struct inode *inode, struct file *filp);

//=============================================================================
// Local Variables: Device handler
//=============================================================================


static char * g_BufferAddr = NULL;
static unsigned int g_BufferLen;
static unsigned int g_BufferPos;
static unsigned int g_BeginPos;

extern ST_DRV_LD_INFO stDrvLdInfo;
extern ST_DRV_LD_PQ_INFO stDrvLdPQInfo;


void MDrv_LD_InitBuff(char *BufferAddr,unsigned int BufLen)
{
    if(NULL == BufferAddr)
    {
        printk("error! %s:%d, parametre error Pointer is null \n", __FUNCTION__, __LINE__);
        return ;
    }
    g_BufferAddr = BufferAddr;
    g_BufferLen = BufLen;
    g_BufferPos = 0;
}

int MDrv_LD_Gets(char * s, int nchar)
{
    int n;
    if ((NULL == s)||(NULL == g_BufferAddr))
    {
        return -1;
    }

    n = 0;
    while(g_BufferPos < g_BufferLen)
    {
        s[n] = g_BufferAddr[g_BufferPos++];
        if (s[n] == '\n')
        {
            s[n+1] = '\0';
            return n;
        }
        n++;
        if (n>=nchar-1)
        {
            s[n] = '\0';
            return n;
        }
    }
    
    if (n)
    {
        s[n] = '\0';
        return n;
    }
    
    return -1;
}

void MDrv_LD_seek(unsigned int pos)
{
    g_BufferPos = pos;
}

int MDrv_LD_Strnicmp(const char *s1, const char *s2, size_t n)
{
    char c1, c2;

    if((NULL == s1)||(NULL == s2))
    {
        printk("error! %s:%d, parametre error Pointer is null \n", __FUNCTION__, __LINE__);
        return -1;
    }
    
    while (n-- != 0 && (*s1 || *s2))
    {
        c1 = *(char *)s1++;
        if ('a' <= c1 && c1 <= 'z')
            c1 += ('A' - 'a');
        c2 = *(char *)s2++;
        if ('a' <= c2 && c2 <= 'z')
            c2 += ('A' - 'a');
        if (c1 != c2)
        return c1 - c2;
    }
    
    return 0;
}

char MDrv_LD_StringToUpperCase(char ch)
{
    return (ch<0x7F)? toupper(ch): ch;
}

unsigned int MDrv_LD_ParseHexU32(char * str)
{
    unsigned int i, ret = 0;

    CHECK_POINTER_I(str);

    if (str[0] == '0' && MDrv_LD_StringToUpperCase(str[1]) == 'X')
        str += 2;
    for (i = 0; i < 8 && (*str != 0); i++, str++) //note: atmost 8*4=32bits
    {
        char ch = MDrv_LD_StringToUpperCase(*str);
        if ('0' <= ch && ch <= '9')
            ret = (ret << 4) | (ch-'0');
        else if ('A' <= ch && ch <= 'F')
            ret = (ret << 4) | (10+ch-'A');
        else
            break;
    }
    
    return ret;
}


static char* MDrv_LD_SkipLeading(const char *str)
{
    if(NULL == str)
    {
        printk("error! %s:%d, parametre error Pointer is null \n", __FUNCTION__, __LINE__);
        return NULL;
    }
    
    while (*str != '\0' && *str <= ' ')
        str++;
    
    return (char *)str;
}

static char* MDrv_LD_SkipTrailing(const char *str, const char *base)
{
    if((NULL == str) || (NULL == base))
    {
        printk("error! %s:%d, parametre error Pointer is null \n", __FUNCTION__, __LINE__);
        return NULL;
    }
    
    while (str > base && *(str-1) <= ' ')
        str--;
    
    return (char *)str;
}

static char* MDrv_LD_StripTrailing(char *str)
{
    char *ptr = MDrv_LD_SkipTrailing(strchr(str, '\0'), str);
    if(NULL == ptr)
    {
        printk("error! %s:%d, parametre error Pointer is null \n", __FUNCTION__, __LINE__);
        return NULL;
    }

    *ptr='\0';
    return str;
}

static int MDrv_LD_GetBeginString(const char *Key)
{
    char *sp, *ep;
    int len, num;
    char LocalBuffer[LINE_BUFFERSIZE];

    /* Move through file 1 line at a time until a section is matched or EOF.  */
    CHECK_POINTER_I(Key);
    len = strlen(Key);
    if (len > 0)
    {
        do
        {
            num = MDrv_LD_Gets(LocalBuffer, LINE_BUFFERSIZE);
            //LD_INFO("MDrv_LD_GetBeginString num:%d  \n", num);
            if (-1 == num)
                return -1;
            sp = MDrv_LD_SkipLeading(LocalBuffer);
            ep = strchr(sp, ']');
        }while (*sp != '[' || ep == NULL || ((int)(ep-sp-1) != len || MDrv_LD_Strnicmp(sp+1,Key,len) != 0));
        g_BeginPos = g_BufferPos;
    }
    else
        return -1;
    
    return 0;
}


static int MDrv_LD_GetKeyString(const char *Key, char *Buffer, int BufferSize)
{
    char *sp, *ep;
    int len, idx, isstring;
    char LocalBuffer[LINE_BUFFERSIZE];

    CHECK_POINTER_I(g_BufferAddr);
    CHECK_POINTER_I(Key);
    CHECK_POINTER_I(Buffer);

    // find the entry. Stop searching upon [end] area.
    len = (int)strlen(Key);
    idx = -1;
    do
    {
        if (!MDrv_LD_Gets(LocalBuffer,LINE_BUFFERSIZE) || *(MDrv_LD_SkipLeading(LocalBuffer)) == '[')
            return -1;
        sp = MDrv_LD_SkipLeading(LocalBuffer);
        ep = strchr(sp, '='); /* Parse out the equal sign */
        if (ep == NULL)
            ep = strchr(sp, ':');
    }while (*sp == ';' || *sp == '#' || ep == NULL || ((int)(MDrv_LD_SkipTrailing(ep,sp)-sp) != len || MDrv_LD_Strnicmp(sp,Key,len) != 0));


    /* Copy up to BufferSize chars to buffer */
    sp = MDrv_LD_SkipLeading(ep + 1);
    isstring = 0;
    for (ep = sp; *ep != '\0' && ((*ep != ';' && *ep != '#') || isstring); ep++)
    {
        if (*ep == '"')
        {
            if (*(ep + 1) == '"')
            ep++;                 /* skip "" (both quotes) */
            else
            isstring = !isstring; /* single quote, toggle isstring */
        }
        else if (*ep == '\\' && *(ep + 1) == '"')
        {
            ep++;                   /* skip \" (both quotes */
        }
    }
    assert(ep != NULL && (*ep == '\0' || *ep == ';' || *ep == '#'));
    *ep = '\0';                 /* terminate at a comment */
    MDrv_LD_StripTrailing(sp);

    strncpy(Buffer,sp,BufferSize);
    Buffer[BufferSize-1]='\0';
    
    return 0;
}

/** MDrv_LD_GetString()
* \param Section     the name of the section to search for
* \param Key         the name of the entry to find the value of
* \param DefValue    default string in the event of a failed read
* \param Buffer      a pointer to the buffer to copy into
* \param BufferSize  the maximum number of characters to copy
*
* \return            the number of characters copied into the supplied buffer
*/
int MDrv_LD_GetString(const char *Key, const char *DefValue, char *Buffer, int BufferSize)
{
    int ok = 0;

    if (Buffer == NULL || DefValue == NULL || BufferSize <= 0 || Key == NULL)
    {
        printk("error! %s:%d, parametre error Pointer is null \n", __FUNCTION__, __LINE__);
        return -1;
    }
    
    if (g_BufferAddr != NULL)
    {
        ok = MDrv_LD_GetKeyString(Key, Buffer, BufferSize);
        MDrv_LD_seek(g_BeginPos);
    }
    
    LD_INFO("MDrv_LD_GetString ok:%d  \n", ok);
    if (-1 == ok)
    {
        strncpy(Buffer, DefValue, BufferSize);
        Buffer[BufferSize-1]='\0';
    }

    return strlen(Buffer);
}

/** MDrv_LD_GetInteger()
* \param Section     the name of the section to search for
* \param Key         the name of the entry to find the value of
* \param DefValue    the default value in the event of a failed read
*
* \return            the value located at Key
*/
long MDrv_LD_GetInteger(const char *Key, long DefValue)
{
    char buff[64];
    int len = 0;
    
    if(NULL == Key)
    {
        printk("error! %s:%d, parametre error Pointer is null \n", __FUNCTION__, __LINE__);
        return DefValue;
    }
    
    len = MDrv_LD_GetString(Key, "", buff, SIZEARRAY(buff));

    if (len == 0)
        return DefValue;

    LD_INFO("BUFF str len:%d  buff = %s \n",len, buff);
    if (buff[0] == '0' && MDrv_LD_StringToUpperCase(buff[1]) == 'X')
        return MDrv_LD_ParseHexU32(buff);
    else
        return simple_strtol(buff,NULL,10);
}

int MDrv_LD_GetBoolean(const char *Key, int DefValue)
{
    char buff[64];
    int len = 0;
    int ret = 0;

    if(NULL == Key)
    {
        printk("error! %s:%d, parametre error Pointer is null \n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    
    len = MDrv_LD_GetString(Key, "", buff, SIZEARRAY(buff));

    if (len == 0)
    {
        return DefValue;
    }

    if((buff[0] == 'y') || (buff[0] == 'Y') || (buff[0] == '1') || (buff[0] == 't') || (buff[0] == 'T'))
    {
        ret = 1 ;
    }
    else if((buff[0] == 'n') || (buff[0] == 'N') || (buff[0] == '0') || (buff[0] == 'f') || (buff[0] == 'F'))
    {
        ret = 0 ;
    }
    else
    {
        ret = DefValue ;
    }
    
    return ret;
}

static void MDrv_LD_GetIniData(void)
{
    int n = 0;
    //begin
    n = MDrv_LD_GetBeginString("begin");
    LD_INFO("MDrv_LD_GetBeginString = %d \n",n);
    //panel
    stDrvLdInfo.u16PanelWidth = MDrv_LD_GetInteger("u16PanelWidth", 3840);
    LD_INFO("u16PanelWidth = %d \n",stDrvLdInfo.u16PanelWidth);
    stDrvLdInfo.u16PanelHeight = MDrv_LD_GetInteger("u16PanelHeight", 2160);
    LD_INFO("u16PanelHeight = %d \n",stDrvLdInfo.u16PanelHeight);
    stDrvLdInfo.u32BaseAddr = MDrv_LD_GetInteger("u32BaseAddr", 0x3e700000);
    LD_INFO("u32BaseAddr = 0x%x \n",(MS_U32)(stDrvLdInfo.u32BaseAddr));
    //pq
    stDrvLdPQInfo.enLEDType = MDrv_LD_GetInteger("eLEDType", 2);
    LD_INFO("eLEDType = %d \n",stDrvLdPQInfo.enLEDType);
    stDrvLdPQInfo.u8LDFWidth = MDrv_LD_GetInteger("u8LDFWidth", 18);
    LD_INFO("u8LDFWidth = %d \n",stDrvLdPQInfo.u8LDFWidth);
    stDrvLdPQInfo.u8LDFHeight = MDrv_LD_GetInteger("u8LDFHeight", 10);
    LD_INFO("u8LDFHeight = %d \n",stDrvLdPQInfo.u8LDFHeight);
    stDrvLdPQInfo.u8LEDWidth = MDrv_LD_GetInteger("u8LEDWidth", 9);
    LD_INFO("u8LEDWidth = %d \n",stDrvLdPQInfo.u8LEDWidth);
    stDrvLdPQInfo.u8LEDHeight = MDrv_LD_GetInteger("u8LEDHeight", 10);
    LD_INFO("u8LEDHeight = %d \n",stDrvLdPQInfo.u8LEDHeight);
    stDrvLdPQInfo.u8LSFWidth = MDrv_LD_GetInteger("u8LSFWidth", 120);
    LD_INFO("u8LSFWidth = %d \n",stDrvLdPQInfo.u8LSFWidth);
    stDrvLdPQInfo.u8LSFHeight = MDrv_LD_GetInteger("u8LSFHeight", 72);
    LD_INFO("u8LSFHeight = %d \n",stDrvLdPQInfo.u8LSFHeight);
    stDrvLdPQInfo.bCompensationEn = MDrv_LD_GetBoolean("bCompEn", 1);    //TRUE
    LD_INFO("bCompEn = %d \n",stDrvLdPQInfo.bCompensationEn);
    stDrvLdPQInfo.bEdge2DEn = MDrv_LD_GetBoolean("bEdge2DEn", 1);    //TRUE
    LD_INFO("bEdge2DEn = %d \n",stDrvLdPQInfo.bEdge2DEn);
    stDrvLdPQInfo.u32LsfInitialPhase_H = MDrv_LD_GetInteger("u32LsfInitialPhase_HL", 0x80000);
    LD_INFO("u32LsfInitialPhase_H = 0x%x \n",stDrvLdPQInfo.u32LsfInitialPhase_H);
    stDrvLdPQInfo.u32LsfInitialPhase_V = MDrv_LD_GetInteger("u32LsfInitialPhase_HR", 0x80000);
    LD_INFO("u32LsfInitialPhase_V = 0x%x \n",stDrvLdPQInfo.u32LsfInitialPhase_V);
    stDrvLdPQInfo.u8DCMaxAlpha = MDrv_LD_GetInteger("u8DCMaxAlpha", 0x8);
    LD_INFO("u8DCMaxAlpha = %d \n",stDrvLdPQInfo.u8DCMaxAlpha);
    stDrvLdPQInfo.u8CompenGain = MDrv_LD_GetInteger("u8CompensationBlend", 0x8);
    LD_INFO("u8CompenGain = %d \n",stDrvLdPQInfo.u8CompenGain);
    //pq sw
    stDrvLdPQInfo.u8GDStrength = MDrv_LD_GetInteger("u8GDStrength", 0xFF);
    LD_INFO("u8GDStrength = %d \n",stDrvLdPQInfo.u8GDStrength);
    stDrvLdPQInfo.u8LDStrength = MDrv_LD_GetInteger("u8LDStrength", 0xFF);
    LD_INFO("u8LDStrength = %d \n",stDrvLdPQInfo.u8LDStrength);
    stDrvLdPQInfo.u8TFStrengthUp = MDrv_LD_GetInteger("u8TFStrengthUp", 0xF6);
    LD_INFO("u8TFStrengthUp = %d \n",stDrvLdPQInfo.u8TFStrengthUp);
    stDrvLdPQInfo.u8TFStrengthDn = MDrv_LD_GetInteger("u8TFStrengthDn", 0xF6);
    LD_INFO("u8TFStrengthDn = %d \n",stDrvLdPQInfo.u8TFStrengthDn);
    stDrvLdPQInfo.u8TFLowThreshold = MDrv_LD_GetInteger("u8TFLowThreshold", 0x14);
    LD_INFO("u8TFLowThreshold = %d \n",stDrvLdPQInfo.u8TFLowThreshold);
    stDrvLdPQInfo.u8TFHightThreshold = MDrv_LD_GetInteger("u8TFHightThreshold", 0x36);
    LD_INFO("u8TFHightThreshold = %d \n",stDrvLdPQInfo.u8TFHightThreshold);
    stDrvLdPQInfo.u8SFStrength = MDrv_LD_GetInteger("u8SFStrength", 0xFF);
    LD_INFO("u8SFStrength = %d \n",stDrvLdPQInfo.u8SFStrength);
    stDrvLdPQInfo.u8SFStrength2 = MDrv_LD_GetInteger("u8SFStrength2", 0xFF);
    LD_INFO("u8SFStrength2 = %d \n",stDrvLdPQInfo.u8SFStrength2);
    stDrvLdPQInfo.u8SFStrength3 = MDrv_LD_GetInteger("u8SFStrength3", 0xFF);
    LD_INFO("u8SFStrength3 = %d \n",stDrvLdPQInfo.u8SFStrength3);
    stDrvLdPQInfo.u8SFStrength4 = MDrv_LD_GetInteger("u8SFStrength4", 0xFF);
    LD_INFO("u8SFStrength4 = %d \n",stDrvLdPQInfo.u8SFStrength4);
    stDrvLdPQInfo.u8SFStrength5 = MDrv_LD_GetInteger("u8SFStrength5", 0xFF);
    LD_INFO("u8SFStrength5 = %d \n",stDrvLdPQInfo.u8SFStrength5);
    stDrvLdPQInfo.u8InGamma = MDrv_LD_GetInteger("u8InGamma", 0x00);
    LD_INFO("u8InGamma = %d \n",stDrvLdPQInfo.u8InGamma);
    stDrvLdPQInfo.u8OutGamma = MDrv_LD_GetInteger("u8OutGamma", 0x00);
    LD_INFO("u8OutGamma = %d \n",stDrvLdPQInfo.u8OutGamma);
    stDrvLdPQInfo.u8BLMinValue = MDrv_LD_GetInteger("u8BLMinValue", 0xFF);
    LD_INFO("u8BLMinValue = %d \n",stDrvLdPQInfo.u8BLMinValue);
    stDrvLdPQInfo.u8NRStrength = MDrv_LD_GetInteger("u8NRStrength", 0x30);
    LD_INFO("u8NRStrength = %d \n",stDrvLdPQInfo.u8NRStrength);
}

MS_BOOL MDrv_LD_ParseIni(char *con_file ,char *con_fileUpdate)
{
    char *buf;
    long n = 0;
	mm_segment_t old_fs;
	struct file *filp;

    CHECK_POINTER_I(con_file);
    CHECK_POINTER_I(con_fileUpdate);
    LD_INFO(" customer path:%s, customer u_path:%s  \n", con_file, con_fileUpdate);
	//Used USB update LDM.ini
    filp = filp_open(con_fileUpdate, O_RDONLY, 0);   
    if(IS_ERR(filp))
    {
        printk("-------------- error! MDrv_LD_ParseIni U fail  filp:%p  \n", filp);
        filp = filp_open(con_file, O_RDONLY , 0);

        if (IS_ERR(filp))
        {
            printk("-------------- error! MDrv_LD_ParseIni config fail filp:%p  \n", filp);
            return FALSE;
        }
    }

    old_fs = get_fs();
    set_fs(KERNEL_DS);
    buf = kzalloc(LDM_BUF_SIZE+1, GFP_KERNEL);
    if(!buf)
    {
        printk("-------------- error! MDrv_LD_ParseIni kzalloc fail buf:%p  \n", buf);
		return FALSE;
    }

    if (filp->f_op && filp->f_op->read)
    {
        n = filp->f_op->read(filp, buf, LDM_BUF_SIZE, &filp->f_pos);
    }

    if (n < 0)
    {
        set_fs(old_fs);
        filp_close(filp, NULL);
        kfree(buf);
        return FALSE;
    }
    
    MDrv_LD_InitBuff(buf,LDM_BUF_SIZE);
    MDrv_LD_GetIniData();

    set_fs(old_fs);
    filp_close(filp, NULL);
    kfree(buf);
    
    return TRUE;
}

MS_BOOL MDrv_LD_CheckData(char *buff, int buff_len)
{
    MS_U16 u16Checked = 0;
    MS_U32 u32Counter = 0;
    MS_U64 u64Sum = 0;

    if(NULL == buff)
    {
        printk("error! %s:%d, parametre error Pointer is null \n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    
    //before checked data    
    for(; u32Counter<LDM_BIN_CHECK_POSITION; u32Counter++)
    {
        u64Sum += *(buff+u32Counter);
    }

    u16Checked = *(buff+LDM_BIN_CHECK_POSITION) + ((*(buff+LDM_BIN_CHECK_POSITION+1))<<8);

    //after checked to 0xbuff_len
    for(u32Counter = LDM_BIN_CHECK_POSITION+2; u32Counter<buff_len; u32Counter++)
    {
        u64Sum += *(buff+u32Counter);
    }

    LD_INFO(" buff_len:%d, u16Checked:0x%x, u64Sum:0x%lx  \n", buff_len, u16Checked,u64Sum);
    if(u16Checked != (u64Sum&0xFFFF))
    {
        return FALSE;
    }

    return TRUE ;
}

MS_BOOL MDrv_LD_ParseBin(char *buff, int buff_len, char *con_file, char *con_fileUpdate)
{
    int n = 0;
    mm_segment_t old_fs;
    struct file *filp;
    MS_BOOL bValid = FALSE;

    CHECK_POINTER_I(buff);
    CHECK_POINTER_I(con_file);
    CHECK_POINTER_I(con_fileUpdate);
    LD_INFO(" customer path:%s, customer u_path:%s  \n", con_file, con_fileUpdate);
	//Used USB update LDM.ini
    filp = filp_open(con_fileUpdate, O_RDONLY, 0);   
    if(IS_ERR(filp))
    {
        printk("-------------- error! MDrv_LD_ParseBin U fail  filp:%p  \n", filp);
        filp = filp_open(con_file, O_RDONLY , 0);

        if (IS_ERR(filp))
        {
            printk("-------------- error! MDrv_LD_ParseBin config fail filp:%p  \n", filp);
            return FALSE;
        }
    }

    old_fs = get_fs();
    set_fs(KERNEL_DS);

    if (filp->f_op && filp->f_op->read)
    {
        n = filp->f_op->read(filp, buff, buff_len, &filp->f_pos);
    }

    bValid = MDrv_LD_CheckData(buff, n);

    if ((n < 0) || (FALSE == bValid))
    {
        set_fs(old_fs);
        filp_close(filp, NULL);
        return FALSE;
    }
    
    set_fs(old_fs);
    filp_close(filp, NULL);
    
    return TRUE;
}

#endif
