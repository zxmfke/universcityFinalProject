
// packageCap20151226Dlg.cpp : 實作檔
//

#include "stdafx.h"
#include "packageCap20151226.h"
#include "packageCap20151226Dlg.h"
#include "DlgProxy.h"
#include "afxdialogex.h"
#include "afxsock.h"
#include "pcap.h"
#include "tchar.h"
#include "inc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define NET_CARD_KEY "System\\CurrentControlSet\\Control\\Network\\{4D36E972-E325-11CE-BFC1-08002BE10318}" //電腦網卡註冊表儲存位置
#define MAX_LINE 1024 //socket最大傳輸量
#define serverIP "120.125.85.134"  //socket server IP
#pragma comment(lib,"ws2_32.lib")

void getDeviceName(CListBox &m_List_device);
void handlePacket(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data);
void Analyse_IPv4Packet(CString eHeader, const u_char *data, CString pTime);
void Analyse_IPv6Package(CString eHeader, const u_char *data, CString pTime);
void Analyse_ARPPacket(CString eHeader, const u_char *data, CString pTime);
void Analyse_PPPoEPackage(const struct pcap_pkthdr *header, struct ether_header *eth, const u_char *data, CString pTime);
void print_UDPPacket(CString eHeader, CString iHeader, const u_char *data, CString pTime);
void print_ICMPPacket(CString eHeader, CString iHeader, const u_char *data, CString pTime);
void print_TCPPacket(CString eHeader, CString iHeader, const u_char *data, CString pTime);
CString print_EthHeader(struct ether_header *eth, const struct pcap_pkthdr *header);
CString print_IPHeader(struct ip4head *IP4Head, struct ip6head *IP6Head, bool isIP4);
CString getTime();
CString buildIP6Addr(struct in6_addr *ip);
void socketSendData(CString data);
DWORD WINAPI CapThread(LPVOID lpParameter);
CListBox m_List_device; //網卡選擇框
//FILE *fp;
pcap_if_t *all_dev = NULL;
pcap_if_t *d = NULL;
pcap_t *pHandle;
HANDLE m_handelThread;
CEdit m_code; //驗證碼edit control
CString code; //驗證碼字串
SOCKET sd; //socket宣告
CSocket aSocket; //

// 對 App About 使用 CAboutDlg 對話方塊

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 對話方塊資料
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

// 程式碼實作
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CpackageCap20151226Dlg 對話方塊


IMPLEMENT_DYNAMIC(CpackageCap20151226Dlg, CDialogEx);

CpackageCap20151226Dlg::CpackageCap20151226Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CpackageCap20151226Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pAutoProxy = NULL;
}

CpackageCap20151226Dlg::~CpackageCap20151226Dlg()
{
	// 如果有此對話方塊的 Automation Proxy，
	// 請將其指向此對話方塊的返回指標設為 NULL，
	// 所以會知道是否已經刪除對話方塊。
	if (m_pAutoProxy != NULL)
		m_pAutoProxy->m_pDialog = NULL;
}

void CpackageCap20151226Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List_device);
	DDX_Control(pDX, IDC_EDIT1, m_code);
}

BEGIN_MESSAGE_MAP(CpackageCap20151226Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CpackageCap20151226Dlg 訊息處理常式

BOOL CpackageCap20151226Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 將 [關於...] 功能表加入系統功能表。

	// IDM_ABOUTBOX 必須在系統命令範圍之中。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	getDeviceName(m_List_device);

	// 設定此對話方塊的圖示。當應用程式的主視窗不是對話方塊時，
	// 框架會自動從事此作業
	SetIcon(m_hIcon, TRUE);			// 設定大圖示
	SetIcon(m_hIcon, FALSE);		// 設定小圖示

	// TODO:  在此加入額外的初始設定

	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}

void CpackageCap20151226Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果將最小化按鈕加入您的對話方塊，您需要下列的程式碼，
// 以便繪製圖示。對於使用文件/檢視模式的 MFC 應用程式，
// 框架會自動完成此作業。

void CpackageCap20151226Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 繪製的裝置內容

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 將圖示置中於用戶端矩形
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 描繪圖示
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 當使用者拖曳最小化視窗時，
// 系統呼叫這個功能取得游標顯示。
HCURSOR CpackageCap20151226Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 如果控制器仍保持其物件之一，而使用者
// 關閉 UI 時，Automation 伺服器不應該結束。
// 這些訊息處理常式會確認是否仍在使用 Proxy，
// 如果已結束使用，則會隱藏 UI，但是對話方塊
// 仍保持在附近。

void CpackageCap20151226Dlg::OnClose()
{
	if (CanExit())
		CDialogEx::OnClose();
}

void CpackageCap20151226Dlg::OnOK()
{
	if (CanExit())
		CDialogEx::OnOK();
}

void CpackageCap20151226Dlg::OnCancel()
{
	if (CanExit())
		CDialogEx::OnCancel();
}

BOOL CpackageCap20151226Dlg::CanExit()
{
	// 如果 Proxy 物件仍在附近，則 Automation 控制器
	// 仍掌控此應用程式。請將對話方塊保持在附近，
	// 但是隱藏其 UI。
	if (m_pAutoProxy != NULL)
	{
		ShowWindow(SW_HIDE);
		return FALSE;
	}

	return TRUE;
}

//按鈕"開始"的動作
void CpackageCap20151226Dlg::OnBnClickedButton1()
{
	AfxMessageBox(_T("開始抓取封包，將驗證碼輸入網頁即可查看"));

	CString deviceSelected;
	int i = 0, j = 0, selectLen = 0;

	for (int i = 0; i < m_List_device.GetCount(); i++){
		if (m_List_device.GetSel(i)){
			m_List_device.GetText(i, deviceSelected);
		}
	}

	for (d = all_dev, i = 0; i < m_List_device.GetCount(); i++){
		if ((CString)d->description != deviceSelected)
			d = d->next;
		else break;
	}

	if (d != NULL){

		pHandle = pcap_open_live(d->name, 65536, PCAP_OPENFLAG_PROMISCUOUS, 500, NULL); //打開所選設備

		if (pHandle == NULL)
		{
			pcap_freealldevs(all_dev);
			AfxMessageBox(_T("pHandle?"));
		}

		printf("\nlistening on %s...\n", d->description);

		pcap_freealldevs(all_dev);

		struct sockaddr_in serv;
		WSADATA wsadata;

		if (WSAStartup(0x101, (LPWSADATA)&wsadata) != 0){
			printf("WSAStartup is fail!");
		}

		//設定socket server的傳輸模式，IP，端口
		serv.sin_family = AF_INET;
		serv.sin_addr.S_un.S_addr = inet_addr("120.125.85.134");
		serv.sin_port = htons(5150);

		//打開一個TCP的socket
		if ((sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == SOCKET_ERROR){
			//MessageBox(NULL, _T("can not open a tcp socket"), _T("hint"), MB_OK);
			MessageBox(_T("can not open a tcp socket"));
			exit(1);
		}

		//連接到server端
		if (connect(sd, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR){
			//MessageBox(NULL, _T("can not connect to server"), _T("hint"), MB_OK);
			MessageBox(_T("can not connect to server"));
			exit(1);
		}

		//建立thread給一個網卡抓取封包
		LPDWORD threadCap = NULL;
		m_handelThread = CreateThread(NULL, 0, CapThread, this, 0, threadCap);
		if (m_handelThread == NULL){
			MessageBox(_T("thread error!"));
		}

	}
	else {
		AfxMessageBox(_T("沒有選擇網卡"));
	}
}

/*************************************************************
* 函數名:getDeviceName

* 函數目的:取得本機所有網卡的名稱，放到listbox裏，供使用者選擇

* 輸入:	 無

* 輸出:  無
*************************************************************/
void getDeviceName(CListBox &m_List_device){

	int i = 0;//設備個數

	//找到所有的設備	
	if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &all_dev, NULL) == -1)
	{
		m_List_device.AddString(_T("no device"));
		return;
	}
	for (d = all_dev; d; d = d->next, ++i)
	{
		m_List_device.AddString((CString)d->description);
	}
	if (i == 0)
	{
		m_List_device.AddString(_T("\nNo network device found! Make sure WinPcap is installed.\n"));
		return;
	}
}

//抓取封包thread
DWORD WINAPI CapThread(LPVOID lpParameter){
	CpackageCap20151226Dlg *pthis = (CpackageCap20151226Dlg*)lpParameter;
	if (m_handelThread == NULL){
		MessageBox(NULL, _T("thread error"), _T("1"), MB_OK);
	}

	pcap_loop(pHandle, 0, handlePacket, NULL);
	return 0;
}

//winpcap中取得每個封包時的回調函數
void handlePacket(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data){
	//MessageBox(NULL,_T("open thread"),_T("hint"),MB_OK);
	struct ether_header *eth = (struct ether_header *)pkt_data;

	CString pTime = getTime();
	CString eHeader = print_EthHeader(eth, header);

	switch (ntohs(eth->ether_type))
	{
	case ETHERTYPE_ARP:
	{
		Analyse_ARPPacket(eHeader, pkt_data + 14, pTime);
		break;
	}
	case ETHERTYPE_IP:
	{
		Analyse_IPv4Packet(eHeader, pkt_data + 14, pTime);
		break;
	}
	case ETHERTYPE_IPv6:
	{
		Analyse_IPv6Package(eHeader, pkt_data + 14, pTime);
		break;
	}
	default:break;
	}
}

/*************************************************************
* 函數名:Analyse_IPv4Packet

* 函數目的:分析IPv4封包，並根據抓取到的協定到所屬函數繼續解封包

* 輸入:	  CString eHeader -- 組建好的乙太封包頭字串
		  const u_char *data -- IPv4頭 data
		  CString pTime -- 取得該封包的時間

* 輸出:  無
*************************************************************/
void Analyse_IPv4Packet(CString eHeader, const u_char *data, CString pTime)
{
	struct ip4head *IP4Head;
	IP4Head = (ip4head *)data;

	CString iHeader = print_IPHeader(IP4Head, NULL, true);

	switch (IP4Head->ip4_protocol)
	{
	case 1:
		print_ICMPPacket(eHeader, iHeader, data + 20, pTime);
		break;
	case 6:
		print_TCPPacket(eHeader, iHeader, data + 20, pTime);
		break;
	case 17:
		print_UDPPacket(eHeader, iHeader, data + 20, pTime);
		break;
	default:
		break;
	}

	return;
}

/*************************************************************
* 函數名:Analyse_IPv6Packet

* 函數目的:分析IPv6封包，並根據抓取到的協定到所屬函數繼續解封包

* 輸入:	  CString eHeader -- 組建好的乙太封包頭字串
		  const u_char *data -- IPv6頭 data
		  CString pTime -- 取得該封包的時間

* 輸出:  無
*************************************************************/
void Analyse_IPv6Package(CString eHeader, const u_char *data, CString pTime){
	struct ip6head  *IP6Head;
	IP6Head = (ip6head *)data;

	CString iHeader = print_IPHeader(NULL, IP6Head, false);

	switch (IP6Head->ip6_next_header)
	{
	case 0x3a:
		print_ICMPPacket(eHeader, iHeader, data + 54, pTime);
		break;
	case 6:
		print_TCPPacket(eHeader, iHeader, data + 20, pTime);
		break;
	case 17:
		print_UDPPacket(eHeader, iHeader, data + 20, pTime);
		break;
	default:
		break;
	}
}

/*************************************************************
* 函數名:Analyse_ARPPacket

* 函數目的:分析ARP封包，並將抓取到的資訊寫入packageLog.txt檔案中

* 輸入:	  CString eHeader -- 組建好的乙太封包頭字串
		  const u_char *data -- arp封包頭資訊
		  CString pTime -- 取得該封包的時間

* 輸出:  無
*************************************************************/
void Analyse_ARPPacket(CString eHeader, const u_char *data, CString pTime)
{
	printf("ARP Package\n");
	CString arpStr;
	u_char* mac_string;
	arpStr.Append(eHeader);
	arpStr.Append(_T(","));

	struct arphead *ARPHead;
	ARPHead = (arphead *)data;

	arpStr.AppendFormat(_T("\"arp\":{\"opcode\":\"%d\","), ntohs(ARPHead->arp_operation_code));
	arpStr.AppendFormat(_T("\"hLen\":\"%d\",\"pLen\":\"%d\","), ARPHead->arp_hardware_length, ARPHead->arp_protocol_length);
	arpStr.AppendFormat(_T("\"hardType\":\"%d\","), ntohs(ARPHead->arp_hardware_type));
	arpStr.AppendFormat(_T("\"proType\":\"%d\","), ntohs(ARPHead->arp_protocol_type));
	arpStr.AppendFormat(_T("\"sip\":\"%d.%d.%d.%d\","), ARPHead->arp_source_ip_address[0], ARPHead->arp_source_ip_address[1], ARPHead->arp_source_ip_address[2], ARPHead->arp_source_ip_address[3]);
	arpStr.AppendFormat(_T("\"dip\":\"%d.%d.%d.%d\","), ARPHead->arp_destination_ip_address[0], ARPHead->arp_destination_ip_address[1], ARPHead->arp_destination_ip_address[2], ARPHead->arp_destination_ip_address[3]);
	mac_string = ARPHead->arp_source_ethernet_address;
	arpStr.AppendFormat(_T("\"smac\":\"%02x:%02x:%02x:%02x:%02x:%02x\","), *mac_string, *(mac_string + 1), *(mac_string + 2), *(mac_string + 3), *(mac_string + 4), *(mac_string + 5));
	mac_string = ARPHead->arp_destination_ethernet_address;
	arpStr.AppendFormat(_T("\"dmac\":\"%02x:%02x:%02x:%02x:%02x:%02x\""), *mac_string, *(mac_string + 1), *(mac_string + 2), *(mac_string + 3), *(mac_string + 4), *(mac_string + 5));

	arpStr.AppendFormat(_T("},\"time\":\"%s\",\"tag\":\"arp\"}\n"), pTime);

	socketSendData(arpStr);

	/*
	fp = fopen("packageLog.txt","at");
	fprintf(fp,arpStr);
	fclose(fp);
	*/
}

/*************************************************************
* 函數名:print_UDPPacket

* 函數目的:分析UDP封包，並將抓取到的資訊組建成字串，傳到函數sockerSendData

* 輸入:	  CString eHeader -- 組建好的乙太封包頭字串
		  CString iHeader -- 組建好的IP頭字串
		  const u_char *data -- udp封包頭資訊
		  CString pTime -- 取得該封包的時間

* 輸出:  無
*************************************************************/
void print_UDPPacket(CString eHeader, CString iHeader, const u_char *data, CString pTime)
{
	printf("UDP Package\n");

	CString udpStr;
	udpStr.AppendFormat(_T("%s,%s,"), eHeader, iHeader);

	struct udphead *UDPHead;
	UDPHead = (udphead *)data;

	udpStr.AppendFormat(_T("\"udp\":{\"sport\":\"%d\",\"dport\":\"%d\",\"len\":\"%d\"},\"time\":\"%s\",\"tag\":\"udp\"}\n"), ntohs(UDPHead->udp_source_port), ntohs(UDPHead->udp_destinanion_port), UDPHead->udp_length, pTime);

	socketSendData(udpStr);

	/*
	fp = fopen("packageLog.txt","at");
	fprintf(fp,udpStr);
	fclose(fp);
	*/
}

/*************************************************************
* 函數名:print_ICMPPacket

* 函數目的:根據eHeader裏面的type，如果是IPv4就解ICMP封包，如果是
IPv6就解ICMPV6封包，並將抓取到的資訊組建成字串，傳到函數sockerSendData

* 輸入:	  CString eHeader -- 組建好的乙太封包頭字串
		  CString iHeader -- 組建好的IP頭字串
		  const u_char *data -- icmp封包頭資訊
		  CString pTime -- 取得該封包的時間

* 輸出:  無
*************************************************************/
void print_ICMPPacket(CString eHeader, CString iHeader, const u_char *data, CString pTime)
{
	printf("ICMP Package\n");
	CString icmpStr;
	icmpStr.AppendFormat(_T("%s,%s,"), eHeader, iHeader);

	if (eHeader.Find(_T("\"type\":\"34525\"")))
	{
		struct icmphead *ICMPHead = (icmphead *)data;
		icmpStr.AppendFormat(_T("\"icmp\":{\"type\":\"%d\",\"code\":\"%d\",\"id\":\"%d\",\"seq\":\"%d\"},\"time\":\"%s\",\"tag\":\"icmp\"}\n"), ICMPHead->icmp_type, ICMPHead->icmp_code, ICMPHead->icmp_id, ICMPHead->icmp_sequence, pTime);
	}
	else
	{
		struct icmpv6head *ICMPv6Head = (icmpv6head *)data;
		icmpStr.AppendFormat(_T("\"icmpv6\":{\"type\":\"%d\",\"code\":\"%d\"},\"time\":\"%s\",\"tag\":\"icmpv6\"}\n"), ICMPv6Head->icmp6_type, ICMPv6Head->icmp6_code, pTime);
	}

	socketSendData(icmpStr);

	/*
	fp = fopen("packageLog.txt","at");
	fprintf(fp,icmpStr);
	fclose(fp);
	*/
}

/*************************************************************
* 函數名:print_TCPPacket

* 函數目的:分析TCP封包，並將抓取到的資訊組建成字串，傳到函數sockerSendData

* 輸入:	  CString eHeader -- 組建好的乙太封包頭字串
		  CString iHeader -- 組建好的IP頭字串
		  const u_char *data -- tcp封包頭資訊
		  CString pTime -- 取得該封包的時間

* 輸出:  無
*************************************************************/
void print_TCPPacket(CString eHeader, CString iHeader, const u_char *data, CString pTime)
{
	printf("TCP Packet\n");
	CString tcpStr;
	tcpStr.AppendFormat(_T("%s,%s,"), eHeader, iHeader);

	struct tcphead *TCPHead;
	TCPHead = (tcphead *)data;

	tcpStr.AppendFormat(_T("\"tcp\":{\"sport\":\"%d\",\"dport\":\"%d\",\"seq\":\"%d\",\"ack\":\"%d\","), ntohs(TCPHead->th_sport), ntohs(TCPHead->th_dport), ntohs(TCPHead->th_seq), ntohs(TCPHead->th_ack));
	tcpStr.AppendFormat(_T("\"flag\":\"%d\",\"win\":\"%d\",\"off\":\"%d\"},\"time\":\"%s\",\"tag\":\"tcp\"}\n"), TCPHead->th_flags, TCPHead->th_win, TCPHead->th_off, pTime);

	socketSendData(tcpStr);

	/*
	fp = fopen("packageLog.txt","at");
	fprintf(fp,tcpStr);
	fclose(fp);
	*/
}

/*************************************************************
* 函數名:print_EthHeader

* 函數目的:拼裝以太網路封包頭資訊

* 輸入:	  struct ether_header *eth --
		  const struct pcap_pkthdr *header --

* 輸出:  拼装好的以太網路封包頭資訊字串
*************************************************************/
CString print_EthHeader(struct ether_header *eth, const struct pcap_pkthdr *header){
	u_char* mac_string;
	CString ethStr;
	ethStr.AppendFormat(_T("{\"Ethernet\":{\"type\":\"%d\",\"len\":\"%d\",\"caplen\":\"%d\","), ntohs(eth->ether_type), header->caplen, header->len);
	mac_string = eth->ether_shost;
	ethStr.AppendFormat(_T("\"smac\":\"%02x:%02x:%02x:%02x:%02x:%02x\","), *mac_string, *(mac_string + 1), *(mac_string + 2), *(mac_string + 3), *(mac_string + 4), *(mac_string + 5));
	mac_string = eth->ether_dhost;
	ethStr.AppendFormat(_T("\"dmac\":\"%02x:%02x:%02x:%02x:%02x:%02x\"}"), *mac_string, *(mac_string + 1), *(mac_string + 2), *(mac_string + 3), *(mac_string + 4), *(mac_string + 5));
	return ethStr;
}

/*************************************************************
* 函數名:print_IPHeader

* 函數目的:根據參數isIP4來判斷是否是IPv4或IPv6，然後拼裝IP封包頭
資訊

* 輸入:	  struct ip4head *IP4Head -- IP4封包頭
	      struct ip6head *IP6Head -- IP6封包頭
		  bool isIP4 -- 判斷是IP4還是IP6

* 輸出:  拼装好的IP封包頭資訊字串
*************************************************************/
CString print_IPHeader(struct ip4head *IP4Head, struct ip6head *IP6Head, bool isIP4){
	CString ipStr;

	if (isIP4)
	{
		struct in_addr *sIP = &(IP4Head->ip4_souce_address);
		struct in_addr *dIP = &(IP4Head->ip4_destination_address);

		ipStr.AppendFormat(_T("\"ip4\":{\"hl\":\"%d\",\"version\":\"%d\",\"len\":\"%d\","), IP4Head->ip4_header_length, IP4Head->ip_version, IP4Head->ip4_length);
		ipStr.AppendFormat(_T("\"sip\":\"%d.%d.%d.%d\","), (int)sIP->S_un.S_un_b.s_b1,
			(int)sIP->S_un.S_un_b.s_b2,
			(int)sIP->S_un.S_un_b.s_b3,
			(int)sIP->S_un.S_un_b.s_b4);

		ipStr.AppendFormat(_T("\"dip\":\"%d.%d.%d.%d\","), (int)dIP->S_un.S_un_b.s_b1,
			(int)dIP->S_un.S_un_b.s_b2,
			(int)dIP->S_un.S_un_b.s_b3,
			(int)dIP->S_un.S_un_b.s_b4);

		ipStr.AppendFormat(_T("\"flag\":\"%d\",\"off\":\"%d\",\"protocol\":\"%d\",\"TTL\":\"%d\"}"), IP4Head->ip4_flag, IP4Head->ip4_off, IP4Head->ip4_protocol, IP4Head->ip4_ttl);
	}
	else
	{
		ipStr.AppendFormat(_T("\"ip6\":{\"version\":\"6\",\"len\":\"%d\",\"label\":\"%d\","), IP6Head->ip6_length, IP6Head->ip6_label);
		ipStr.AppendFormat(_T("\"nextHeader\":\"%d\",\"hopLimit\":\"%d\","), IP6Head->ip6_next_header, IP6Head->ip6_limits);
		ipStr.AppendFormat(_T("\"sip\":\"%s\",\"dip\":\"%s\"}"), buildIP6Addr(&(IP6Head->ip6_souce_address)), buildIP6Addr(&(IP6Head->ip6_destination_address)));

	}

	return ipStr;
}

/*************************************************************
* 函數名:getTime

* 函數目的:讀取抓到封包時的system time

* 輸入:	  無

* 輸出:  以"%d%02d%02d%02d%02d%02d%03d"格式拼裝的時間字串
*************************************************************/
CString getTime(){
	SYSTEMTIME st;
	CString timeStr;
	GetLocalTime(&st);
	timeStr.Format(_T("%d%02d%02d%02d%02d%02d%03d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	return timeStr;
}

/*************************************************************
* 函數名:buildIP6Addr

* 函數目的:化簡並拼裝IPv6的IP地址

* 輸入:	  struct in6_addr *ip -- 封包IP

* 輸出:  拼裝好的IPv6封包的IP地址字串
*************************************************************/
CString buildIP6Addr(struct in6_addr *ip){

	CString str;
	CString tstr;
	CString zero = _T("");
	bool judge[9];
	int parallelism[8][2] = { { 0, 1 }, { 2, 3 }, { 4, 5 }, { 6, 7 }, { 8, 9 }, { 10, 11 }, { 12, 13 }, { 14, 15 } };
	bool isUse = false;
	bool haveMTOZero = false;

	for (int i = 0; i<9; i++){
		judge[i] = (i != 8) ? ((ip->u.Byte[parallelism[i][0]]) || (ip->u.Byte[parallelism[i][1]]) ? true : false) : true;
	}

	//化簡
	for (int i = 0; i<8; i++){
		if ((judge[i] || judge[i + 1]) || isUse){
			if (!haveMTOZero || isUse)
			{
				unsigned char evenNum = ip->u.Byte[parallelism[i][0]];
				unsigned char oddNum = ip->u.Byte[parallelism[i][1]];
				if (evenNum)
					str.AppendFormat(_T("%x"), evenNum);
				if (oddNum)
				{
					if (oddNum>17)str.AppendFormat(_T("%x"), oddNum);
					else {
						str.AppendFormat(evenNum == 0 ? _T("%x") : _T("%02x"), oddNum);
					}
				}
				else str.Append(_T("00"));

				str.Append((i != 7) ? _T(":") : _T(""));
			}
			else
			{
				str.Append(!judge[0] ? _T("::") : _T(":"));
				isUse = true;
				haveMTOZero = false;
			}
		}
		else {
			haveMTOZero = true;
		}
	}

	return str;
}

/*************************************************************
* 函數名:socketSendData

* 函數目的:將組裝好的封包資訊傳送給socket server

* 輸入:	  CString data -- 封包資訊

* 輸出:  無
*************************************************************/
void socketSendData(CString data){

	//將CString轉換成const char
	data.AppendFormat(_T("-%s"), code);
	const wchar_t* buf = (LPCTSTR)data;
	char str[1024] = { 0 };
	wcstombs(str, buf, wcslen(buf));

	//傳送資訊
	int judge = send(sd, str, sizeof(str) + 1, 0);
	if (judge < 0){
		printf("fail send");
		exit(1);
	}
}

//按鈕"停止"的動作
void CpackageCap20151226Dlg::OnBnClickedButton2()
{
	MessageBox(_T("停止"));
	if (m_handelThread == NULL){
		return;
	}
	if (TerminateThread(m_handelThread, -1) == 0){
		MessageBox(_T("thread shutdown error"));
		return;
	}
	aSocket.Close();
	m_handelThread = NULL;
}