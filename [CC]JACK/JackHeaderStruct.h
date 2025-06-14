
//#pragma once

#include <string>

// JackHeader Struct Version Info
//#define JACKV_DEV		0x00
//#define JACKV_ALPHA		0x01
//#define JACKV_BETA		0x02
//#define JACKV_RC_RTM	0x03

//#define	JACKSVERSION(a,b,c,d)	((a<<24)+(b<<16)+(c<<8)+d)//(M,m,p,s1,s2,b) (M&0x0F)<<28|(m&0x0F)<<24|(p&0x1F)<<19|(s1&0x03)<<17|(s2&0x07)<<14|(b&0x3FFF)

//#define TRINITY_VER		JACKSVERSION(1,3,1,20)//(4,0,12,JACKV_RC_RTM,1,79)

#define THEUNDERSCORE "\xc9"
#define MASTERVERSION "\xa4\xa4"//"\xa7\xa9"
#define PARTOFMONTH "\xaf"
#define PARTOFWEEK "\xa5\xa6"//"\xa5\xa4"
#define _NEWJACK_
#define _LIKEPLAYPARK
#define _ITEMFUSE


/*ALL NEW UPDATE UNLOCKED*/
#define _NAGA_
#define _ITEMLINK_
#define _ITEMDROPINFO_
#define _MPAGES_
#define _LUNAAUTOPATCH_
//#define _OLDGOLDDIALOG_
//#define _GOLDDLGONEBTN_
#define _EDDAAUTOPATCH_
#define _SKILLSLOT_
/**/

//----------------------------------------------------------
//Expiry date
//----------------------------------------------------------
/*#ifdef _TIMEBACK_
#define	tahun	2022	//tahun
#define bulan	10		//bulan ///tak perlu guna 01 02 etc... guna 1 2
#define tanggal		1	//tanggal (sampai 2359, masuk tanggal seterusnya baru block)
//#define hariapa	5	//hari (dalam minggu 0-ahad)	
#endif*/




//----------------------------------------------------------
//Inventory extra M pages
//----------------------------------------------------------
#ifdef _MPAGES_
#define MPAGESSLOT 40
#else
#define MPAGESSLOT 0
#endif

//----------------------------------------------------------
//Itemdrop info dialog
//----------------------------------------------------------
#ifdef _ITEMDROPINFO_
#define DISABLEROPDIALOG 0
#define DROPINFOPATH "./Resource/MonsterDropItemList.bin"
#else
#define DISABLEROPDIALOG 1
#define DROPINFOPATH "./Resource/Server/MonsterDropItemList.bin"
#endif





//----------------------------------------------------------
//ClientVerInfo.ver
//----------------------------------------------------------
#define TESTTEST "\xb8\xbe\xca\xf2\xfa\xf3\xf5\xab\xd9\xf5\xe4\xd8\xe5\xfc\xe2\xd1\xe0\xf4\xfb"
#define CLIENTBY "\xd5\xfd\xe0\xfb\xfd\xe2\xbb\xbd\xf6\xb0\xdc\xf0\xe8\xf1"












#ifdef _EDDAAUTOPATCH_
#ifdef _LUNAAUTOPATCH_
#undef _LUNAAUTOPATCH_
#endif
#endif
#ifdef _NZMSOMSO_
#include <../[CC]JACK/Users/NZMSOMSO>
#endif
