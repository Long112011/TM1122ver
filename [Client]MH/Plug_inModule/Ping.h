////////////////////////////////////////////////////////////////////
//                          _ooOoo_                               //
//                         o8888888o                              //
//                         88" . "88                              //
//                         (| ^_^ |)                              //
//                         O\  =  /O                              //
//                      ____/`---'\____                           //
//                    .'  \\|     |//  `.                         //
//                   /  \\|||  :  |||//  \                        //
//                  /  _||||| -:- |||||-  \                       //
//                  |   | \\\  -  /// |   |                       //
//                  | \_|  ''\---/''  |   |                       //
//                  \  .-\__  `-`  ___/-. /                       //
//                ___`. .'  /--.--\  `. . ___                     //
//             ."" '<  `.___\_<|>_/___.'  >'"".                   //
//            | | :  `- \`.;`\ _ /`;.`/ - ` : | |                 //
//            \  \ `-.   \_ __\ /__ _/   .-` /  /                 //
//     ========`-.____`-.___\_____/___.-`____.-'========          //
//                          `=---='                               //
//     ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^         //
//         佛祖保佑       永无BUG     永不修改					  //
////////////////////////////////////////////////////////////////////
#include <IPHlpApi.h>
#pragma comment(lib, "iphlpapi.lib")

class CPing
{
public:
	CPing(void);
	~CPing(void);
private:
	HINSTANCE hIcmp;
	typedef HANDLE(WINAPI *PIcmpCreateFile)(VOID);
	typedef DWORD(WINAPI *PIcmpSendEcho)(HANDLE, IPAddr, LPVOID, WORD, PIP_OPTION_INFORMATION, LPVOID, DWORD, DWORD);
	typedef BOOL(WINAPI *PIcmpCloseHandle)(HANDLE);
	PIcmpCreateFile        pIcmpCreateFile;
	PIcmpSendEcho        pIcmpSendEcho;
	PIcmpCloseHandle    pIcmpCloseHandle;
public:
	//[Function: ping an address or IP and return the result]
	//[Parameter: v_pszDesAddress-target domain name or IP]
	//[Parameter: v_usTimes-number of pings required]
	//[Parameter: v_ulTimeout-timeout time, in milliseconds]
	//[Return value: -1-an exception occurred, other values greater than or equal to 0 indicate the number of times that have not timed out]
	int Ping(const char *v_pszDesAdress, USHORT v_usTimes, ULONG v_ulTimeout,DWORD & time);
};