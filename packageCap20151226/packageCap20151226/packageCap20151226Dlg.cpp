
// packageCap20151226Dlg.cpp : �����n
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

#define NET_CARD_KEY "System\\CurrentControlSet\\Control\\Network\\{4D36E972-E325-11CE-BFC1-08002BE10318}" //��X�W���]�Ա탦��λ��
#define MAX_LINE 1024 //socket����ݔ��
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
CListBox m_List_device; //�W���x���
//FILE *fp;
pcap_if_t *all_dev = NULL;
pcap_if_t *d = NULL;
pcap_t *pHandle;
HANDLE m_handelThread;
CEdit m_code; //��C�aedit control
CString code; //��C�a�ִ�
SOCKET sd; //socket����
CSocket aSocket; //

// �� App About ʹ�� CAboutDlg ��Ԓ���K

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��Ԓ���K�Y��
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧Ԯ

// ��ʽ�a����
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


// CpackageCap20151226Dlg ��Ԓ���K


IMPLEMENT_DYNAMIC(CpackageCap20151226Dlg, CDialogEx);

CpackageCap20151226Dlg::CpackageCap20151226Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CpackageCap20151226Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pAutoProxy = NULL;
}

CpackageCap20151226Dlg::~CpackageCap20151226Dlg()
{
	// ����дˌ�Ԓ���K�� Automation Proxy��
	// Ո����ָ��ˌ�Ԓ���K�ķ���ָ���O�� NULL��
	// ���ԕ�֪���Ƿ��ѽ��h����Ԓ���K��
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


// CpackageCap20151226Dlg ӍϢ̎��ʽ

BOOL CpackageCap20151226Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �� [�P�...] ���ܱ����ϵ�y���ܱ�

	// IDM_ABOUTBOX �����ϵ�y�����֮�С�
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

	// �O���ˌ�Ԓ���K�ĈDʾ�������ó�ʽ����ҕ�����ǌ�Ԓ���K�r��
	// ��ܕ��Ԅӏ��´����I
	SetIcon(m_hIcon, TRUE);			// �O����Dʾ
	SetIcon(m_hIcon, FALSE);		// �O��С�Dʾ

	// TODO:  �ڴ˼����~��ĳ�ʼ�O��

	return TRUE;  // ���� TRUE����������������O�����c
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

// �������С�����o�������Č�Ԓ���K������Ҫ���еĳ�ʽ�a��
// �Ա��L�u�Dʾ�����ʹ���ļ�/�zҕģʽ�� MFC ���ó�ʽ��
// ��ܕ��Ԅ���ɴ����I��

void CpackageCap20151226Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �L�u���b�Ã���

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ���Dʾ������Ñ��˾���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ���L�Dʾ
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ��ʹ������ҷ��С��ҕ���r��
// ϵ�y�����@������ȡ���Θ��@ʾ��
HCURSOR CpackageCap20151226Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// ����������Ա��������֮һ����ʹ����
// �P�] UI �r��Automation �ŷ�������ԓ�Y����
// �@ЩӍϢ̎��ʽ���_�J�Ƿ�����ʹ�� Proxy��
// ����ѽY��ʹ�ã��t���[�� UI�����ǌ�Ԓ���K
// �Ա����ڸ�����

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
	// ��� Proxy ������ڸ������t Automation ������
	// ���ƿشˑ��ó�ʽ��Ո����Ԓ���K�����ڸ�����
	// �����[���� UI��
	if (m_pAutoProxy != NULL)
	{
		ShowWindow(SW_HIDE);
		return FALSE;
	}

	return TRUE;
}

//���o"�_ʼ"�Ą���
void CpackageCap20151226Dlg::OnBnClickedButton1()
{
	AfxMessageBox(_T("�_ʼץȡ���������C�aݔ��W퓼��ɲ鿴"));

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

		pHandle = pcap_open_live(d->name, 65536, PCAP_OPENFLAG_PROMISCUOUS, 500, NULL); //���_���x�O��

		if (pHandle == NULL)
		{
			pcap_freealldevs(all_dev);
			AfxMessageBox(_T("pHandle�X?"));
		}

		printf("\nlistening on %s...\n", d->description);

		pcap_freealldevs(all_dev);

		struct sockaddr_in serv;
		WSADATA wsadata;

		if (WSAStartup(0x101, (LPWSADATA)&wsadata) != 0){
			printf("WSAStartup is fail!");
		}

		//�O��socket server�Ă�ݔģʽ��IP���˿�
		serv.sin_family = AF_INET;
		serv.sin_addr.S_un.S_addr = inet_addr("120.125.85.134");
		serv.sin_port = htons(5150);

		//���_һ��TCP��socket
		if ((sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == SOCKET_ERROR){
			//MessageBox(NULL, _T("can not open a tcp socket"), _T("hint"), MB_OK);
			MessageBox(_T("can not open a tcp socket"));
			exit(1);
		}

		//�B�ӵ�server��
		if (connect(sd, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR){
			//MessageBox(NULL, _T("can not connect to server"), _T("hint"), MB_OK);
			MessageBox(_T("can not connect to server"));
			exit(1);
		}

		//����thread�oһ���W��ץȡ���
		LPDWORD threadCap = NULL;
		m_handelThread = CreateThread(NULL, 0, CapThread, this, 0, threadCap);
		if (m_handelThread == NULL){
			MessageBox(_T("thread error!"));
		}

	}
	else {
		AfxMessageBox(_T("�]���x��W��"));
	}
}

/*************************************************************
* ������:getDeviceName

* ����Ŀ��:ȡ�ñ��C���оW�������Q���ŵ�listbox�Y����ʹ�����x��

* ݔ��:	 �o

* ݔ��:  �o
*************************************************************/
void getDeviceName(CListBox &m_List_device){

	int i = 0;//�O�䂀��

	//�ҵ����е��O��	
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

//ץȡ���thread
DWORD WINAPI CapThread(LPVOID lpParameter){
	CpackageCap20151226Dlg *pthis = (CpackageCap20151226Dlg*)lpParameter;
	if (m_handelThread == NULL){
		MessageBox(NULL, _T("thread error"), _T("1"), MB_OK);
	}

	pcap_loop(pHandle, 0, handlePacket, NULL);
	return 0;
}

//winpcap��ȡ��ÿ������r�Ļ��{����
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
* ������:Analyse_IPv4Packet

* ����Ŀ��:����IPv4������K����ץȡ���ąf�������ٺ����^�m����

* ݔ��:	  CString eHeader -- �M���õ���̫����^�ִ�
		  const u_char *data -- IPv4�^ data
		  CString pTime -- ȡ��ԓ����ĕr�g

* ݔ��:  �o
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
* ������:Analyse_IPv6Packet

* ����Ŀ��:����IPv6������K����ץȡ���ąf�������ٺ����^�m����

* ݔ��:	  CString eHeader -- �M���õ���̫����^�ִ�
		  const u_char *data -- IPv6�^ data
		  CString pTime -- ȡ��ԓ����ĕr�g

* ݔ��:  �o
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
* ������:Analyse_ARPPacket

* ����Ŀ��:����ARP������K��ץȡ�����YӍ����packageLog.txt�n����

* ݔ��:	  CString eHeader -- �M���õ���̫����^�ִ�
		  const u_char *data -- arp����^�YӍ
		  CString pTime -- ȡ��ԓ����ĕr�g

* ݔ��:  �o
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
* ������:print_UDPPacket

* ����Ŀ��:����UDP������K��ץȡ�����YӍ�M�����ִ�����������sockerSendData

* ݔ��:	  CString eHeader -- �M���õ���̫����^�ִ�
		  CString iHeader -- �M���õ�IP�^�ִ�
		  const u_char *data -- udp����^�YӍ
		  CString pTime -- ȡ��ԓ����ĕr�g

* ݔ��:  �o
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
* ������:print_ICMPPacket

* ����Ŀ��:����eHeader�Y���type�������IPv4�ͽ�ICMP����������
IPv6�ͽ�ICMPV6������K��ץȡ�����YӍ�M�����ִ�����������sockerSendData

* ݔ��:	  CString eHeader -- �M���õ���̫����^�ִ�
		  CString iHeader -- �M���õ�IP�^�ִ�
		  const u_char *data -- icmp����^�YӍ
		  CString pTime -- ȡ��ԓ����ĕr�g

* ݔ��:  �o
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
* ������:print_TCPPacket

* ����Ŀ��:����TCP������K��ץȡ�����YӍ�M�����ִ�����������sockerSendData

* ݔ��:	  CString eHeader -- �M���õ���̫����^�ִ�
		  CString iHeader -- �M���õ�IP�^�ִ�
		  const u_char *data -- tcp����^�YӍ
		  CString pTime -- ȡ��ԓ����ĕr�g

* ݔ��:  �o
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
* ������:print_EthHeader

* ����Ŀ��:ƴ�b��̫�W·����^�YӍ

* ݔ��:	  struct ether_header *eth --
		  const struct pcap_pkthdr *header --

* ݔ��:  ƴװ�õ���̫�W·����^�YӍ�ִ�
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
* ������:print_IPHeader

* ����Ŀ��:��������isIP4���Д��Ƿ���IPv4��IPv6��Ȼ��ƴ�bIP����^
�YӍ

* ݔ��:	  struct ip4head *IP4Head -- IP4����^
	      struct ip6head *IP6Head -- IP6����^
		  bool isIP4 -- �Д���IP4߀��IP6

* ݔ��:  ƴװ�õ�IP����^�YӍ�ִ�
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
* ������:getTime

* ����Ŀ��:�xȡץ������r��system time

* ݔ��:	  �o

* ݔ��:  ��"%d%02d%02d%02d%02d%02d%03d"��ʽƴ�b�ĕr�g�ִ�
*************************************************************/
CString getTime(){
	SYSTEMTIME st;
	CString timeStr;
	GetLocalTime(&st);
	timeStr.Format(_T("%d%02d%02d%02d%02d%02d%03d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	return timeStr;
}

/*************************************************************
* ������:buildIP6Addr

* ����Ŀ��:�����Kƴ�bIPv6��IP��ַ

* ݔ��:	  struct in6_addr *ip -- ���IP

* ݔ��:  ƴ�b�õ�IPv6�����IP��ַ�ִ�
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

	//����
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
* ������:socketSendData

* ����Ŀ��:���M�b�õķ���YӍ���ͽosocket server

* ݔ��:	  CString data -- ����YӍ

* ݔ��:  �o
*************************************************************/
void socketSendData(CString data){

	//��CString�D�Q��const char
	data.AppendFormat(_T("-%s"), code);
	const wchar_t* buf = (LPCTSTR)data;
	char str[1024] = { 0 };
	wcstombs(str, buf, wcslen(buf));

	//�����YӍ
	int judge = send(sd, str, sizeof(str) + 1, 0);
	if (judge < 0){
		printf("fail send");
		exit(1);
	}
}

//���o"ֹͣ"�Ą���
void CpackageCap20151226Dlg::OnBnClickedButton2()
{
	MessageBox(_T("ֹͣ"));
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