import sys
import xlrd

###enter file name
#excel_filename_PWS_table=raw_input('Enter PWS excel fle:')
#if excel_filename_PWS_table == '' :
#    print "Input Error....\n"
#    sys.exit()

###Excel file handle
# book_pws = xlrd.open_workbook(excel_filename_PWS_table)
book_spi = xlrd.open_workbook("Flash_Device_Info.xls")
#print "The number of sheets is ", book_pws.nsheets
#print "Sheet(s) name(s):", book_pws.sheet_names()

#print "input_source_list dir ", input_source_list
###text file handle
#open file (pws_setting_info_table.h)
spi_table_file = open('drvDeviceInfo.h','w')
spi_table_file.write('#ifndef _DRV_DEVICE_INFO_H_\n')
spi_table_file.write('#define _DRV_DEVICE_INFO_H_\n\n')
spi_table_file.write('#include \"MsTypes.h\"\n\n')


#get sheet info sheet
sheet_list_info = book_spi.sheet_by_name("Sheet_Info")
sheet_IP_list = sheet_list_info.col_values(0, 2, None)
sheet_VendorList = sheet_list_info.col_values(1, 2, None)
sheet_SizeList = sheet_list_info.col_values(2, 2, None)
sheet_ProtectTableList = sheet_list_info.col_values(3, 2, None)


for x in sheet_ProtectTableList:
   sheet_IP = book_spi.sheet_by_name(x)
   sheet_IP_DeviceName = sheet_IP.col_values(1, 2, None)
   sheet_IP_DeviceNum = sheet_IP.col_values(39, 2, None)
   sheet_IP_DeviceName_length = len(sheet_IP_DeviceName) -1 #zero-based
   sheet_IP_DeviceNum_length = len(sheet_IP_DeviceNum) -1 #zero-based
   sheet_IP_index = 0
   while sheet_IP_index <= sheet_IP_DeviceName_length :
        spi_table_file.write('#define FLASH_IC_' + sheet_IP_DeviceName[sheet_IP_index]+'    '+sheet_IP_DeviceNum[sheet_IP_index]+ '\n' )
        sheet_IP_index = sheet_IP_index +1

spi_table_file.write('\n' )



for x in sheet_VendorList:
   sheetVendorList = book_spi.sheet_by_name(x)
   sheet_IP_Vendor = sheetVendorList.col_values(1, 2, None)
   sheet_IP_ManufactureID = sheetVendorList.col_values(2, 2, None)
   sheet_IP_Vendor_length = len(sheet_IP_Vendor) -1 #zero-based
   sheet_IP_ManufactureID_length = len(sheet_IP_ManufactureID) -1 #zero-based
   sheet_IP_index = 0
   while sheet_IP_index <= sheet_IP_Vendor_length :
        spi_table_file.write('#define  ' + sheet_IP_Vendor[sheet_IP_index]+'    '+sheet_IP_ManufactureID[sheet_IP_index]+ '\n' )
        sheet_IP_index = sheet_IP_index +1

spi_table_file.write('\n' )


for x in sheet_SizeList:
   sheetSizeList = book_spi.sheet_by_name(x)
   sheet_IP_Size = sheetSizeList.col_values(1, 2, None)
   sheet_IP_Hex = sheetSizeList.col_values(2, 2, None)
   sheet_IP_Size_length = len(sheet_IP_Size) -1 #zero-based
   sheet_IP_Hex_length = len(sheet_IP_Hex) -1 #zero-based
   sheet_IP_index = 0
   while sheet_IP_index <= sheet_IP_Size_length :
        spi_table_file.write('#define SIZE_' + sheet_IP_Size[sheet_IP_index]+'    '+sheet_IP_Hex[sheet_IP_index]+ '\n' )
        sheet_IP_index = sheet_IP_index +1


spi_table_file.write('\n' )

spi_table_file.write('#define ISP_DEV_PMC             BITS(2:0, 0)\n')
spi_table_file.write('#define ISP_DEV_NEXTFLASH       BITS(2:0, 1)\n')
spi_table_file.write('#define ISP_DEV_ST              BITS(2:0, 2)\n')
spi_table_file.write('#define ISP_DEV_SST             BITS(2:0, 3)\n')
spi_table_file.write('#define ISP_DEV_ATMEL           BITS(2:0, 4)\n')
spi_table_file.write('#define ISP_SPI_ENDIAN_BIG      BITS(0:0, 1)\n')
spi_table_file.write('#define ISP_SPI_ENDIAN_LITTLE   BITS(0:0, 0)\n')

spi_table_file.write('\n' )

#write pws data structure
spi_table_file.write('typedef   struct   {\n')
spi_table_file.write('    MS_U8   u8BlockProtectBits;\n')
spi_table_file.write('    MS_U32  u32LowerBound;\n')
spi_table_file.write('    MS_U32  u32UpperBound;\n')
spi_table_file.write('}ST_WRITE_PROTECT;\n\n')

#write pws data structure
spi_table_file.write('typedef   struct   {\n')
spi_table_file.write('    MS_U16  u16Start;\n')
spi_table_file.write('    MS_U16  u16End;\n')
spi_table_file.write('    MS_U32  au32SizeList[8];\n')
spi_table_file.write('}ST_SPECIAL_BLOCKS;\n\n')

#write pws data structure
spi_table_file.write('typedef   enum   {\n')
spi_table_file.write('    E_SINGLE_MODE,\n')
spi_table_file.write('    E_FAST_MODE,\n')
spi_table_file.write('    E_DUAL_D_MODE,\n')
spi_table_file.write('    E_DUAL_AD_MODE,\n')
spi_table_file.write('    E_QUAD_MODE,\n')
spi_table_file.write('    E_ISP_MODE,\n')
spi_table_file.write('}SPI_READ_MODE;\n\n')

#write pws data structure
spi_table_file.write('typedef   struct   {\n')
spi_table_file.write('    MS_U16  u16FlashType;\n')
spi_table_file.write('    MS_U8   u8MID;\n')
spi_table_file.write('    MS_U8   u8DID0;\n')
spi_table_file.write('    MS_U8   u8DID1;\n')
spi_table_file.write('    ST_WRITE_PROTECT   *pWriteProtectTable;\n')
spi_table_file.write('    ST_SPECIAL_BLOCKS  *pSpecialBlocks;\n')
spi_table_file.write('    MS_U32  u32FlashSize;\n')
spi_table_file.write('    MS_U32  u32NumSec;\n')
spi_table_file.write('    MS_U32  u32SecSize;\n')
spi_table_file.write('    MS_U16  u16PageSize;\n')
spi_table_file.write('    MS_U16  u16MaxChipWrDoneTimeout;\n')
spi_table_file.write('    MS_U8   u8WrsrBlkProtect;\n')
spi_table_file.write('    MS_U16  u16DevSel;\n')
spi_table_file.write('    MS_U16  u16SpiEndianSel;\n')
spi_table_file.write('    MS_U16  u16SPIMaxClk[2];\n')
spi_table_file.write('    MS_BOOL b2XREAD;\n')
spi_table_file.write('    MS_U16   u16WrQuadEnable;\n')
spi_table_file.write('}hal_SERFLASH_t;\n\n')

#write pws data structure
spi_table_file.write('static ST_SPECIAL_BLOCKS _stSpecialBlocks_EN25F10 =\n')
spi_table_file.write('{\n')
spi_table_file.write('    .u16Start     = 0,\n')
spi_table_file.write('    .u16End       = 3,\n')
spi_table_file.write('    .au32SizeList =\n')
spi_table_file.write('    { \n')
spi_table_file.write('        SIZE_32KB,\n')
spi_table_file.write('        SIZE_32KB,\n')
spi_table_file.write('        SIZE_32KB,\n')
spi_table_file.write('        SIZE_32KB,\n')
spi_table_file.write('     }\n')
spi_table_file.write('};\n\n')

#write pws data structure
spi_table_file.write('static ST_SPECIAL_BLOCKS _stSpecialBlocks_EN25B32B =\n')
spi_table_file.write('{\n')
spi_table_file.write('    .u16Start     = 0,\n')
spi_table_file.write('    .u16End       = 3,\n')
spi_table_file.write('    .au32SizeList =\n')
spi_table_file.write('    { \n')
spi_table_file.write('        SIZE_4KB,\n')
spi_table_file.write('        SIZE_4KB,\n')
spi_table_file.write('        SIZE_8KB,\n')
spi_table_file.write('        SIZE_16KB,\n')
spi_table_file.write('        SIZE_32KB,\n')
spi_table_file.write('     }\n')
spi_table_file.write('};\n\n')

#write pws data structure
spi_table_file.write('static ST_SPECIAL_BLOCKS _stSpecialBlocks_EN25B64B =\n')
spi_table_file.write('{\n')
spi_table_file.write('    .u16Start     = 0,\n')
spi_table_file.write('    .u16End       = 3,\n')
spi_table_file.write('    .au32SizeList =\n')
spi_table_file.write('    { \n')
spi_table_file.write('        SIZE_4KB,\n')
spi_table_file.write('        SIZE_4KB,\n')
spi_table_file.write('        SIZE_8KB,\n')
spi_table_file.write('        SIZE_16KB,\n')
spi_table_file.write('        SIZE_32KB,\n')
spi_table_file.write('     }\n')
spi_table_file.write('};\n\n')

spi_table_file.write('#define FROM_BLK(x, blk_size)   ((x) * (blk_size))\n')
spi_table_file.write('#define TO_BLK(x, blk_size)     (((x) + 1) * (blk_size) - 1)\n')
spi_table_file.write('\n' )

for x in sheet_ProtectTableList:
   sheetProtectTableList = book_spi.sheet_by_name(x)
   sheet_IP_Name = sheetProtectTableList.col_values(1, 2, None)
   sheet_IP_ProtectTable = sheetProtectTableList.col_values(2, 2, None)
   sheet_IP_SpecialProtect = sheetProtectTableList.col_values(3, 2, None)
   sheet_IP_WrBlkBph = sheetProtectTableList.col_values(5, 2, None)
   sheet_IP_WrBlkBpL = sheetProtectTableList.col_values(6, 2, None)
   sheet_Pro0L = sheetProtectTableList.col_values(7, 2, None)
   sheet_Pro0H = sheetProtectTableList.col_values(8, 2, None)
   sheet_Pro1L = sheetProtectTableList.col_values(9, 2, None)
   sheet_Pro1H = sheetProtectTableList.col_values(10, 2, None)
   sheet_Pro2L = sheetProtectTableList.col_values(11, 2, None)
   sheet_Pro2H = sheetProtectTableList.col_values(12, 2, None)
   sheet_Pro3L = sheetProtectTableList.col_values(13, 2, None)
   sheet_Pro3H = sheetProtectTableList.col_values(14, 2, None)
   sheet_Pro4L = sheetProtectTableList.col_values(15, 2, None)
   sheet_Pro4H = sheetProtectTableList.col_values(16, 2, None)
   sheet_Pro5L = sheetProtectTableList.col_values(17, 2, None)
   sheet_Pro5H = sheetProtectTableList.col_values(18, 2, None)
   sheet_Pro6L = sheetProtectTableList.col_values(19, 2, None)
   sheet_Pro6H = sheetProtectTableList.col_values(20, 2, None)
   sheet_Pro7L = sheetProtectTableList.col_values(21, 2, None)
   sheet_Pro7H = sheetProtectTableList.col_values(22, 2, None)
   sheet_Pro8L = sheetProtectTableList.col_values(23, 2, None)
   sheet_Pro8H = sheetProtectTableList.col_values(24, 2, None)
   sheet_Pro9L = sheetProtectTableList.col_values(25, 2, None)
   sheet_Pro9H = sheetProtectTableList.col_values(26, 2, None)
   sheet_Pro10L = sheetProtectTableList.col_values(27, 2, None)
   sheet_Pro10H = sheetProtectTableList.col_values(28, 2, None)
   sheet_Pro11L = sheetProtectTableList.col_values(29, 2, None)
   sheet_Pro11H = sheetProtectTableList.col_values(30, 2, None)
   sheet_Pro12L = sheetProtectTableList.col_values(31, 2, None)
   sheet_Pro12H = sheetProtectTableList.col_values(32, 2, None)
   sheet_Pro13L = sheetProtectTableList.col_values(33, 2, None)
   sheet_Pro13H = sheetProtectTableList.col_values(34, 2, None)
   sheet_Pro14L = sheetProtectTableList.col_values(35, 2, None)
   sheet_Pro14H = sheetProtectTableList.col_values(36, 2, None)
   sheet_Pro15L = sheetProtectTableList.col_values(37, 2, None)
   sheet_Pro15H = sheetProtectTableList.col_values(38, 2, None)
   sheet_ProtectName = sheetProtectTableList.col_values(40, 2, None) 
   sheet_IP_Name_length = len(sheet_IP_Name) -1 #zero-based
   sheet_IP_ProtectTable_length = len(sheet_IP_ProtectTable) -1 #zero-based
   sheet_IP_index = 0
   while sheet_IP_index <= sheet_IP_Name_length :
         if sheet_IP_ProtectTable[sheet_IP_index] == "Y" and sheet_IP_SpecialProtect[sheet_IP_index] == "N" :
            spi_table_file.write('static ST_WRITE_PROTECT   _pstWriteProtectTable_' + sheet_IP_Name[sheet_IP_index])
            spi_table_file.write('[] =\n')
            spi_table_file.write('{\n')
            spi_table_file.write('    {  BITS('+str(int(sheet_IP_WrBlkBph[sheet_IP_index]))+':'+str(int(sheet_IP_WrBlkBpL[sheet_IP_index]))+', 0),  '+sheet_Pro0L[sheet_IP_index]+',        '+sheet_Pro0H[sheet_IP_index]+'            },\n' )
            spi_table_file.write('    {  BITS('+str(int(sheet_IP_WrBlkBph[sheet_IP_index]))+':'+str(int(sheet_IP_WrBlkBpL[sheet_IP_index]))+', 1),  FROM_BLK('+str(int(sheet_Pro1L[sheet_IP_index]))+', SIZE_64KB),  TO_BLK('+str(int(sheet_Pro1H[sheet_IP_index]))+', SIZE_64KB)  },\n')
            spi_table_file.write('    {  BITS('+str(int(sheet_IP_WrBlkBph[sheet_IP_index]))+':'+str(int(sheet_IP_WrBlkBpL[sheet_IP_index]))+', 2),  FROM_BLK('+str(int(sheet_Pro2L[sheet_IP_index]))+', SIZE_64KB),  TO_BLK('+str(int(sheet_Pro2H[sheet_IP_index]))+', SIZE_64KB)  },\n')
            spi_table_file.write('    {  BITS('+str(int(sheet_IP_WrBlkBph[sheet_IP_index]))+':'+str(int(sheet_IP_WrBlkBpL[sheet_IP_index]))+', 3),  FROM_BLK('+str(int(sheet_Pro3L[sheet_IP_index]))+', SIZE_64KB),  TO_BLK('+str(int(sheet_Pro3H[sheet_IP_index]))+', SIZE_64KB)  },\n')
            spi_table_file.write('    {  BITS('+str(int(sheet_IP_WrBlkBph[sheet_IP_index]))+':'+str(int(sheet_IP_WrBlkBpL[sheet_IP_index]))+', 4),  FROM_BLK('+str(int(sheet_Pro4L[sheet_IP_index]))+', SIZE_64KB),  TO_BLK('+str(int(sheet_Pro4H[sheet_IP_index]))+', SIZE_64KB)  },\n')
            spi_table_file.write('    {  BITS('+str(int(sheet_IP_WrBlkBph[sheet_IP_index]))+':'+str(int(sheet_IP_WrBlkBpL[sheet_IP_index]))+', 5),  FROM_BLK('+str(int(sheet_Pro5L[sheet_IP_index]))+', SIZE_64KB),  TO_BLK('+str(int(sheet_Pro5H[sheet_IP_index]))+', SIZE_64KB)  },\n')
            spi_table_file.write('    {  BITS('+str(int(sheet_IP_WrBlkBph[sheet_IP_index]))+':'+str(int(sheet_IP_WrBlkBpL[sheet_IP_index]))+', 6),  FROM_BLK('+str(int(sheet_Pro6L[sheet_IP_index]))+', SIZE_64KB),  TO_BLK('+str(int(sheet_Pro6H[sheet_IP_index]))+', SIZE_64KB)  },\n')
            if sheet_Pro7L[sheet_IP_index] !="N":
              spi_table_file.write('    {  BITS('+str(int(sheet_IP_WrBlkBph[sheet_IP_index]))+':'+str(int(sheet_IP_WrBlkBpL[sheet_IP_index]))+', 7),  FROM_BLK('+str(int(sheet_Pro7L[sheet_IP_index]))+', SIZE_64KB),  TO_BLK('+str(int(sheet_Pro7H[sheet_IP_index]))+', SIZE_64KB)  },\n')
            if sheet_Pro8L[sheet_IP_index] !="N":
              spi_table_file.write('    {  BITS('+str(int(sheet_IP_WrBlkBph[sheet_IP_index]))+':'+str(int(sheet_IP_WrBlkBpL[sheet_IP_index]))+', 8),  FROM_BLK('+str(int(sheet_Pro8L[sheet_IP_index]))+', SIZE_64KB),  TO_BLK('+str(int(sheet_Pro8H[sheet_IP_index]))+', SIZE_64KB)  },\n')
            if sheet_Pro9L[sheet_IP_index] !="N":
              spi_table_file.write('    {  BITS('+str(int(sheet_IP_WrBlkBph[sheet_IP_index]))+':'+str(int(sheet_IP_WrBlkBpL[sheet_IP_index]))+', 9),  FROM_BLK('+str(int(sheet_Pro9L[sheet_IP_index]))+', SIZE_64KB),  TO_BLK('+str(int(sheet_Pro9H[sheet_IP_index]))+', SIZE_64KB)  },\n')
            if sheet_Pro10L[sheet_IP_index] !="N":
              spi_table_file.write('    {  BITS('+str(int(sheet_IP_WrBlkBph[sheet_IP_index]))+':'+str(int(sheet_IP_WrBlkBpL[sheet_IP_index]))+', 10),  FROM_BLK('+str(int(sheet_Pro10L[sheet_IP_index]))+', SIZE_64KB),  TO_BLK('+str(int(sheet_Pro10H[sheet_IP_index]))+', SIZE_64KB)  },\n')
            if sheet_Pro11L[sheet_IP_index] !="N":
              spi_table_file.write('    {  BITS('+str(int(sheet_IP_WrBlkBph[sheet_IP_index]))+':'+str(int(sheet_IP_WrBlkBpL[sheet_IP_index]))+', 11),  FROM_BLK('+str(int(sheet_Pro11L[sheet_IP_index]))+', SIZE_64KB),  TO_BLK('+str(int(sheet_Pro11H[sheet_IP_index]))+', SIZE_64KB)  },\n')
            if sheet_Pro12L[sheet_IP_index] !="N":
              spi_table_file.write('    {  BITS('+str(int(sheet_IP_WrBlkBph[sheet_IP_index]))+':'+str(int(sheet_IP_WrBlkBpL[sheet_IP_index]))+', 12),  FROM_BLK('+str(int(sheet_Pro12L[sheet_IP_index]))+', SIZE_64KB),  TO_BLK('+str(int(sheet_Pro12H[sheet_IP_index]))+', SIZE_64KB)  },\n')
            if sheet_Pro13L[sheet_IP_index] !="N":
              spi_table_file.write('    {  BITS('+str(int(sheet_IP_WrBlkBph[sheet_IP_index]))+':'+str(int(sheet_IP_WrBlkBpL[sheet_IP_index]))+', 13),  FROM_BLK('+str(int(sheet_Pro13L[sheet_IP_index]))+', SIZE_64KB),  TO_BLK('+str(int(sheet_Pro13H[sheet_IP_index]))+', SIZE_64KB)  },\n')
            if sheet_Pro14L[sheet_IP_index] !="N":
              spi_table_file.write('    {  BITS('+str(int(sheet_IP_WrBlkBph[sheet_IP_index]))+':'+str(int(sheet_IP_WrBlkBpL[sheet_IP_index]))+', 14),  FROM_BLK('+str(int(sheet_Pro14L[sheet_IP_index]))+', SIZE_64KB),  TO_BLK('+str(int(sheet_Pro14H[sheet_IP_index]))+', SIZE_64KB)  },\n')
            if sheet_Pro15L[sheet_IP_index] !="N":
              spi_table_file.write('    {  BITS('+str(int(sheet_IP_WrBlkBph[sheet_IP_index]))+':'+str(int(sheet_IP_WrBlkBpL[sheet_IP_index]))+', 15),  FROM_BLK('+str(int(sheet_Pro15L[sheet_IP_index]))+', SIZE_64KB),  TO_BLK('+str(int(sheet_Pro15H[sheet_IP_index]))+', SIZE_64KB)  },\n')                                                                                        
            spi_table_file.write('};\n')
         else :
            if sheet_IP_ProtectTable[sheet_IP_index] == "Y" and sheet_IP_SpecialProtect[sheet_IP_index] == "Y" :
               spi_table_file.write('static ST_WRITE_PROTECT   _pstWriteProtectTable_' + sheet_IP_DeviceName[sheet_IP_index]+'[] =\n' )
               spi_table_file.write('{\n')
               spi_table_file.write('    {  BITS('+str(int(sheet_IP_WrBlkBph[sheet_IP_index]))+':'+str(int(sheet_IP_WrBlkBpL[sheet_IP_index]))+', 0),  '+sheet_Pro0L[sheet_IP_index]+',        '+sheet_Pro0H[sheet_IP_index]+'            },\n' )
               spi_table_file.write('    {  BITS('+str(int(sheet_IP_WrBlkBph[sheet_IP_index]))+':'+str(int(sheet_IP_WrBlkBpL[sheet_IP_index]))+', 1),  '+str(sheet_Pro1L[sheet_IP_index])+',        '+str(sheet_Pro1H[sheet_IP_index])+'            },\n')
               spi_table_file.write('    {  BITS('+str(int(sheet_IP_WrBlkBph[sheet_IP_index]))+':'+str(int(sheet_IP_WrBlkBpL[sheet_IP_index]))+', 2),  '+str(sheet_Pro2L[sheet_IP_index])+',        '+str(sheet_Pro2H[sheet_IP_index])+'            },\n')
               spi_table_file.write('    {  BITS('+str(int(sheet_IP_WrBlkBph[sheet_IP_index]))+':'+str(int(sheet_IP_WrBlkBpL[sheet_IP_index]))+', 3),  '+str(sheet_Pro3L[sheet_IP_index])+',        '+str(sheet_Pro3H[sheet_IP_index])+'            },\n')
               spi_table_file.write('    {  BITS('+str(int(sheet_IP_WrBlkBph[sheet_IP_index]))+':'+str(int(sheet_IP_WrBlkBpL[sheet_IP_index]))+', 4),  '+str(sheet_Pro4L[sheet_IP_index])+',        '+str(sheet_Pro4H[sheet_IP_index])+'            },\n')
               if sheet_Pro5L[sheet_IP_index] !="N":
                  spi_table_file.write('    {  BITS('+str(int(sheet_IP_WrBlkBph[sheet_IP_index]))+':'+str(int(sheet_IP_WrBlkBpL[sheet_IP_index]))+', 5),  '+str(sheet_Pro5L[sheet_IP_index])+',        '+str(sheet_Pro5H[sheet_IP_index])+'            },\n')
               if sheet_Pro6L[sheet_IP_index] !="N": 
                  spi_table_file.write('    {  BITS('+str(int(sheet_IP_WrBlkBph[sheet_IP_index]))+':'+str(int(sheet_IP_WrBlkBpL[sheet_IP_index]))+', 6),  '+str(sheet_Pro6L[sheet_IP_index])+',        '+str(sheet_Pro6H[sheet_IP_index])+'            },\n')
               if sheet_Pro7L[sheet_IP_index] !="N":
                  spi_table_file.write('    {  BITS('+str(int(sheet_IP_WrBlkBph[sheet_IP_index]))+':'+str(int(sheet_IP_WrBlkBpL[sheet_IP_index]))+', 7),  '+str(sheet_Pro7L[sheet_IP_index])+',        '+str(sheet_Pro7H[sheet_IP_index])+'            },\n')
               if sheet_Pro8L[sheet_IP_index] !="N": 
                  spi_table_file.write('    {  BITS('+str(int(sheet_IP_WrBlkBph[sheet_IP_index]))+':'+str(int(sheet_IP_WrBlkBpL[sheet_IP_index]))+', 8),  '+str(sheet_Pro8L[sheet_IP_index])+',        '+str(sheet_Pro8H[sheet_IP_index])+'            },\n')
               if sheet_Pro9L[sheet_IP_index] !="N":
                  spi_table_file.write('    {  BITS('+str(int(sheet_IP_WrBlkBph[sheet_IP_index]))+':'+str(int(sheet_IP_WrBlkBpL[sheet_IP_index]))+', 9),  '+str(sheet_Pro9L[sheet_IP_index])+',        '+str(sheet_Pro9H[sheet_IP_index])+'            },\n')
               if sheet_Pro10L[sheet_IP_index] !="N": 
                  spi_table_file.write('    {  BITS('+str(int(sheet_IP_WrBlkBph[sheet_IP_index]))+':'+str(int(sheet_IP_WrBlkBpL[sheet_IP_index]))+', 10),  '+str(sheet_Pro10L[sheet_IP_index])+',        '+str(sheet_Pro10H[sheet_IP_index])+'            },\n')
               if sheet_Pro11L[sheet_IP_index] !="N":
                  spi_table_file.write('    {  BITS('+str(int(sheet_IP_WrBlkBph[sheet_IP_index]))+':'+str(int(sheet_IP_WrBlkBpL[sheet_IP_index]))+', 11),  '+str(sheet_Pro11L[sheet_IP_index])+',        '+str(sheet_Pro11H[sheet_IP_index])+'            },\n')
               if sheet_Pro12L[sheet_IP_index] !="N": 
                  spi_table_file.write('    {  BITS('+str(int(sheet_IP_WrBlkBph[sheet_IP_index]))+':'+str(int(sheet_IP_WrBlkBpL[sheet_IP_index]))+', 12),  '+str(sheet_Pro12L[sheet_IP_index])+',        '+str(sheet_Pro12H[sheet_IP_index])+'            },\n')
               if sheet_Pro13L[sheet_IP_index] !="N":
                  spi_table_file.write('    {  BITS('+str(int(sheet_IP_WrBlkBph[sheet_IP_index]))+':'+str(int(sheet_IP_WrBlkBpL[sheet_IP_index]))+', 13),  '+str(sheet_Pro13L[sheet_IP_index])+',        '+str(sheet_Pro13H[sheet_IP_index])+'            },\n')
               if sheet_Pro14L[sheet_IP_index] !="N": 
                  spi_table_file.write('    {  BITS('+str(int(sheet_IP_WrBlkBph[sheet_IP_index]))+':'+str(int(sheet_IP_WrBlkBpL[sheet_IP_index]))+', 14),  '+str(sheet_Pro14L[sheet_IP_index])+',        '+str(sheet_Pro14H[sheet_IP_index])+'            },\n')
               if sheet_Pro15L[sheet_IP_index] !="N":
                  spi_table_file.write('    {  BITS('+str(int(sheet_IP_WrBlkBph[sheet_IP_index]))+':'+str(int(sheet_IP_WrBlkBpL[sheet_IP_index]))+', 15),  '+str(sheet_Pro15L[sheet_IP_index])+',        '+str(sheet_Pro15H[sheet_IP_index])+'            },\n')
               spi_table_file.write('};\n')
               spi_table_file.write('\n' )
         sheet_IP_index = sheet_IP_index +1

spi_table_file.write('\n' )
spi_table_file.write('\n' )
spi_table_file.write('\n' )
a  =  "_pstWriteProtectTable_"
b  =  ""
c  =  "NULL,"
d  =  "&_stSpecialBlocks_"
aString_length=len(a)
cString_length=len(c)
dString_length=len(d)
for x in sheet_IP_list:
   sheet_DeviceVendor = book_spi.sheet_by_name(x)
   sheet_DeviceString = sheet_DeviceVendor.col_values(1, 2, None)
   sheet_VendorString = sheet_DeviceVendor.col_values(2, 2, None)
   sheet_DeviceNum = sheet_DeviceVendor.col_values(3, 2, None)
   sheet_DID0 = sheet_DeviceVendor.col_values(4, 2, None)
   sheet_DID1 = sheet_DeviceVendor.col_values(5, 2, None)
   sheet_ProtectTable = sheet_DeviceVendor.col_values(6, 2, None)
   sheet_SpecialBlocks= sheet_DeviceVendor.col_values(7, 2, None)
   sheet_FlashSize= sheet_DeviceVendor.col_values(8, 2, None)
   sheet_NumBLK= sheet_DeviceVendor.col_values(9, 2, None)
   sheet_BlockSize= sheet_DeviceVendor.col_values(10, 2, None)
   sheet_PageSize= sheet_DeviceVendor.col_values(11, 2, None)
   sheet_MaxChipWrTimeout= sheet_DeviceVendor.col_values(12, 2, None)
   sheet_BpH= sheet_DeviceVendor.col_values(13, 2, None)
   sheet_BpL= sheet_DeviceVendor.col_values(14, 2, None)
   sheet_ProLength= sheet_DeviceVendor.col_values(15, 2, None)
   sheet_DevSel= sheet_DeviceVendor.col_values(16, 2, None)
   sheet_SpiEndianSel= sheet_DeviceVendor.col_values(17, 2, None)
   sheet_SpiClk= sheet_DeviceVendor.col_values(18, 2, None)
   sheet_SpiReadMode= sheet_DeviceVendor.col_values(19, 2, None)
   sheet_RiuRead= sheet_DeviceVendor.col_values(20, 2, None)
   sheet_QuadReadSts= sheet_DeviceVendor.col_values(21, 2, None)
   sheep_CMP = sheet_DeviceVendor.col_values(22, 2, None)
   sheet_DeviceString_length = len(sheet_DeviceString) -1 #zero-based
   
   sheet_IP_index = 0
   str_buf_Max=0
   str_DeviceStringLength=0
   str_buf_VendorLength_Max=0
   str_VendorStringLength=0
   str_buf_SizeStringLen_Max=0
   str_SizeStringLength=0
   str_buf_NumBlkLength_Max=0
   str_NumBlkStringLength=0
   str_buf_MaxTimeoutLength_Max=0
   str_MaxTimeoutStringLength=0
   str_buf_DevSelLength_Max =0
   str_devSelStringLength =0 
   while sheet_IP_index <= sheet_DeviceString_length :
       str_DeviceStringLength=len(sheet_DeviceString[sheet_IP_index])
       str_VendorStringLength=len(sheet_VendorString[sheet_IP_index])
       str_SizeStringLength=len(sheet_FlashSize[sheet_IP_index])
       str_NumBlkStringLength=len(str(int(sheet_NumBLK[sheet_IP_index])))
       str_MaxTimeoutStringLength=len(str(int(sheet_MaxChipWrTimeout[sheet_IP_index])))
       str_DevSelStringLength = len(sheet_DevSel[sheet_IP_index])
       if str_DeviceStringLength > str_buf_Max :
         str_buf_Max = str_DeviceStringLength
       if str_VendorStringLength > str_buf_VendorLength_Max :
         str_buf_VendorLength_Max = str_VendorStringLength
       if str_SizeStringLength > str_buf_SizeStringLen_Max :
         str_buf_SizeStringLen_Max = str_SizeStringLength
       if str_NumBlkStringLength > str_buf_NumBlkLength_Max :
         str_buf_NumBlkLength_Max =  str_NumBlkStringLength
       if str_MaxTimeoutStringLength > str_buf_MaxTimeoutLength_Max :
         str_buf_MaxTimeoutLength_Max =  str_MaxTimeoutStringLength
       if str_devSelStringLength > str_buf_DevSelLength_Max :
         str_buf_DevSelLength_Max = str_devSelStringLength
         
       sheet_IP_index = sheet_IP_index +1

   print(str_buf_Max)
   print(str_buf_VendorLength_Max)

   sheet_IP_index = 0
   str_len_tmp = 0
   spi_table_file.write('static hal_SERFLASH_t _hal_SERFLASH_table[] =\n')
   spi_table_file.write('{ \n')   
   while sheet_IP_index <= sheet_DeviceString_length :  
        spi_table_file.write('     { FLASH_IC_' + sheet_DeviceString[sheet_IP_index]+',')
        #Reorder Vendor String
        str_len_tmp = len(sheet_DeviceString[sheet_IP_index])
        str_device_length_dif = str_buf_Max - str_len_tmp
        spi_table_file.write(b.rjust(5)+b.rjust(str_device_length_dif))
        #Reorder Device String
        str_len_tmp = len(sheet_VendorString[sheet_IP_index])
        str_vendor_length_dif = str_buf_VendorLength_Max - str_len_tmp
        spi_table_file.write(sheet_VendorString[sheet_IP_index]+',')
        spi_table_file.write(b.rjust(5)+b.rjust(str_vendor_length_dif))
        spi_table_file.write(str(sheet_DID0[sheet_IP_index])+',     '+str(sheet_DID1[sheet_IP_index])+',   ')
        if sheet_ProtectTable[sheet_IP_index] !="N":
           spi_table_file.write(a.rjust(10)+sheet_DeviceString[sheet_IP_index]+', ')
        else :
           spi_table_file.write('NULL,')
        if sheet_SpecialBlocks[sheet_IP_index] !="N":
           spi_table_file.write(b.rjust(aString_length)+b.rjust(str_device_length_dif)+b.rjust(cString_length))
           spi_table_file.write('&_stSpecialBlocks_'+sheet_DeviceString[sheet_IP_index]+', ')
        else :
           if sheet_ProtectTable[sheet_IP_index] !="N":
               spi_table_file.write(b.rjust(str_device_length_dif))
           else :
               spi_table_file.write(b.rjust(aString_length)+b.rjust(cString_length)+b.rjust(7))
           spi_table_file.write('NULL,')
        if  sheet_SpecialBlocks[sheet_IP_index] =="N":
            spi_table_file.write(b.rjust(cString_length)+b.rjust(dString_length))
        #Reorder Size String
        str_len_tmp = len(sheet_FlashSize[sheet_IP_index])
        str_size_length_dif = str_buf_SizeStringLen_Max - str_len_tmp
        spi_table_file.write(str(sheet_FlashSize[sheet_IP_index])+',')
        spi_table_file.write(b.rjust(str_size_length_dif)+b.rjust(cString_length))
        spi_table_file.write(str(int(sheet_NumBLK[sheet_IP_index]))+',')
        #Reorder NumBlk String
        str_len_tmp = len(str(int(sheet_NumBLK[sheet_IP_index])))
        str_numblk_length_dif = str_buf_NumBlkLength_Max - str_len_tmp
        spi_table_file.write(b.rjust(str_numblk_length_dif)+b.rjust(cString_length))
        spi_table_file.write(sheet_BlockSize[sheet_IP_index]+',    '+str(int(sheet_PageSize[sheet_IP_index]))+',   '+str(int(sheet_MaxChipWrTimeout[sheet_IP_index]))+',')
        #Reorder MaxClock String
        str_len_tmp = len(str(int(sheet_MaxChipWrTimeout[sheet_IP_index])))
        str_MaxClk_length_dif=str_buf_MaxTimeoutLength_Max - str_len_tmp
        spi_table_file.write(b.rjust(str_MaxClk_length_dif)+b.rjust(cString_length))
        spi_table_file.write('BITS('+str(int(sheet_BpH[sheet_IP_index]))+':'+str(int(sheet_BpL[sheet_IP_index]))+',  '+str(sheet_ProLength[sheet_IP_index])+'),      ')
        spi_table_file.write(sheet_DevSel[sheet_IP_index]+',')
        #Reorder DevSel String
        str_len_tmp =len(sheet_DevSel[sheet_IP_index])
        str_DevSel_length_dif=str_buf_DevSelLength_Max-str_len_tmp
        spi_table_file.write(b.rjust(str_DevSel_length_dif)+b.rjust(cString_length))
        spi_table_file.write('ISP_SPI_ENDIAN_'+sheet_SpiEndianSel[sheet_IP_index]+',    {')
        spi_table_file.write('E_SPI_'+str(sheet_SpiClk[sheet_IP_index])+' , E_'+sheet_SpiReadMode[sheet_IP_index]+'  }, ')
        if sheet_RiuRead[sheet_IP_index] =="N":
           spi_table_file.write('FALSE, ')
        else :
           spi_table_file.write('TRUE, ')
        if  str(int(sheet_QuadReadSts[sheet_IP_index])) =="0" :
           spi_table_file.write('0 },   ')
        else :
             spi_table_file.write('BITS('+str(int(sheet_QuadReadSts[sheet_IP_index]))+':'+str(int(sheet_QuadReadSts[sheet_IP_index]))+', 1)},')       
        spi_table_file.write('\n' )           
        sheet_IP_index = sheet_IP_index +1
        
spi_table_file.write('};\n')
spi_table_file.write('#endif\n')                

print "Table generating OK...!!!"




