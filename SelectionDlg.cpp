
//****************************************************************//
//*   This is the BeijingHell Main user interface system and     *//
//*   events handling system.                                    *//
//*                                                              *//
//*   Author  : Guo xianghao                                     *//
//*   Date    : 2000-2001                                        *//
//*   E-mail  : guoxh@hotmail.com                                *//
//*   All rights reserved.                                       *//
//    Guoly computing company.                                    //
//****************************************************************//
/*
        History:   
version 1.2.2  for Windows XP:
           real-time news causes game can not run under XP.
		   disable it using define
version 1.1.0   2001/2/25
            fix another bug regarding to price.
            some new features added, including: hacking, house agency, network club,
            "boss coming " protect etc.
version 1.0.3   2001/1/9 
            fix the bug.
            put on www.csdn.net, and game.sina.com.cn provides free download, more than
			21500 downlaods now.
version 1.0.2,  2001/1/6
            there's a bug related to goods mangement, Zhangjian found it.
            Fixed.
created: 2000/12

*/




#include "stdafx.h"
#include <mmsystem.h>          //for play sound
#include <io.h>                //for get filelength
#include "DualListDemo.h"
#include "SelectionDlg.h"
#include "BuyDlg.h"
#include "SellDlg.h"
#include "ListCtrlSortClass.h"
#include "EnterBank.h"
#include "Hispital.h"
#include "ReplayLoad.h"
#include "NewsDlg.h"
#include "RijiDlg.h"
#include "NetworkClub.h"
#include "TopPlayerDlg.h"
#include "Wangba.h"
#include "DLGHTML.h"
#include "HouseDlg.h"
#include "RichTop10.h"
#include "HyperLink.h"
#include "SetupGame.h"
#include "StoryDlg.h"
#include "BeijingIntroDlg.h"
#include "BossComeDlg.h"
#include "PostMoneyDlg.h"
#include "ShengmingDlg.h"
#include "BitmapPicture.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define BEIJING     1
#define SHANGHAI    2

// when this is defined, real_time news is disabled.
//#define FOR_WINDOWS_XP 1 



/*
    get min of two int
 */
int MIN(int x, int y)
{
	if(x>y)
		return y;
	else
		return x;
}

/* 
      get a random number ranged from 0 to upper
*/
int  RandomNum(int upper);

int  RandomNum(int upper)
{
	static int i=0;
	if(i==0){
	srand((unsigned)(time(NULL)));
	i=1;
	}
	return rand()%upper;

};

#define GAME_MESSAGE_COUNT		18      // normal event number

/*
   Random events type.
*/
typedef struct MessageType {
	int freq;   // the frequency of the events 
	char *msg;  // what message to display when event happen
	int drug;  // goods ID to be influenced
	int plus;  // price increased ( *)
	int minus; //price decrease   ( /)
	int add;   // how many goods to give user (+) 
} Message;


Message gameMessages [GAME_MESSAGE_COUNT] = {
	{170, "ר��������ߴ�ѧ�����������ʡ�������������ܻ�ӭ!", 5, 2, 0, 0},
	{139, "�����Ժ���˵���������ô����ҩ���ȼٰ׾ƣ��綾���Ϳ���!", 3, 3, 0, 0},
	{100, "ҽԺ�����ܱ��棺�����Ϻ�С��������Ч����ΰ�硱!", 4, 5, 0, 0},
	{41, "��ä˵����2000��ŵ������ѧ�����ޣ��������VCD��̨Ƭ����", 2, 4, 0, 0},
	{37, "����������С�������ۣ�����˽���������ƽ���������!��", 1, 3, 0, 0},
	{23, "���������������ۣ����ᳫ�������䵽ʵ������α�ӻ�ױƷ���ܻ�ӭ!", 7, 4, 0, 0},
	{37, "8858.com�������Ҳ���������Ϻ�С������������һ��������!", 4, 8, 0, 0},
	{15, "л�����������˵�����ҿ�!��ʹ��α�ӻ�ױƷ!����α�ӻ�ױƷ����Ӧ��!", 7, 7, 0, 0},
	{40, "�������˿���ɽ���پƣ������������!", 3, 7, 0, 0},
	{29, "�����Ĵ�ѧ���ǿ�ʼ�ҹ�����ˮ���ֻ����ܻ�ӭ��!", 6, 7, 0, 0},
	{35, "�����ĸ��˷��ع�����˽�������۸����!", 1, 8, 0, 0},
	{17, "�г��ϳ�������Ը�������˽����!", 0, 0, 8, 0},
	{24, "�����ĺ����Ƕ�æ������ѧϰ���������û��Ը����", 5, 0, 5, 0},
	{18, "����ҵʮ�����������й���ȡ������йش�ȫ��������VCD�Ĵ��!", 2, 0, 8, 0},
	{160, "���ŵ���ͬѧ������������˽���������ˣ���", 1, 0, 0, 2},
	{45, "���̾�ɨ���󣬰��ںڰ������﷢�������綪ʧ�Ľ������̡�", 0, 0, 0, 6},
	{35, "������ؼ�ǰ��һЩɽ���ٰ׾ƣ��綾������!", 3, 0, 0, 4},
	{140,"ý�屨���������ձ����ڵ��й��Ĳ�Ʒ������! ���º��ձ�����������,�ܾ��⳥���峤��֪����Ϣ�����˰����õ�ˮ���ֻ������κγ��̱�ʶ��Ӳ������������2500Ԫ��",6,0,0,1}
};

// bad events user may encounter which do harm to user's health

typedef struct EventType {
	int freq;     // the frequency of this event
	char *msg;    // the message to dispplay while event happen
	int hunt;     // how many points user get hurted when event happen
	char *sound;  // the sound file to play for the event
} BadEvent;

#define EVENT_CNT    12    // total such events 

BadEvent random_event [EVENT_CNT] = {
	{117, "�����������å���˰�!", 3,"kill.wav"},
	{157, "���ڹ��ֵص����˴����ɹ�! ", 20,"death.wav"},
	{21, "���ֵ̾�׷������������ͬ�� ",1,"dog.wav"},
	{100, "����ӵ���Ľ�ͨ�ð��Ľ�! ",1,"harley.wav"},
	{35, "��С�͵Ĵ�һ����!",1,"hit.wav"},
	{313, "һȺ�񹤴��˰�!", 10,"flee.wav"},
	{120, "������ͬ��һ��С�����Ұ�һשͷ!", 5,"death.wav"},
	{29, "����д��¥һ���ٱ����õ�������!",3,"el.wav"},
	{43, "�������ڵ�С��Ѭ������! ",1,"vomit.wav"},
	{45, "�����г�����������Ц��û��������!",1,"level.wav"},
	{48, "��������40��!����...",1,"lan.wav"},
	{33, "��������·羰����������ɳ����!",1,"breath.wav"}
};

/*
     The event that has influency to user's money
*/
typedef struct StealType {
	int freq;  //the frequency of the event
	char *msg; //the message to display when event happen
	int ratoi;  // how many ratio decreased. money=money*(1-ratoi)
} StealEvent;

#define STEAL_EVENT_CNT    7   //total numbers of such events

StealEvent random_steal_event [STEAL_EVENT_CNT] = {

	{60, "�����������ڰ��ݳ���ؤ����̫̫��", 10},
	{125, "һ�������ڽ�ͷ��ס���������ǣ�����Ǯ��!����",10},
	{100, "һ����������˰�һ�£�˵��������!����",40},
	{65, "�����������µ���̫̫��ס���������������?����!��",20},
	{35, "�������о�ס���������������ӷѡ������ѡ���", 15},
	{27, "������˵�����쾭��֤?���ϲ�Ҫȥ�ҼҸ�����ǮŶ����", 10},
	{40, "����������Ⱦ������,��ȥ��������...", 5}
};

// about dialog

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
    CHyperLink m_urlEMail;         //  my email address( hyperlink type)
    CHyperLink m_urlGroups;        //  news group of BeijingHell

	CHyperLink m_homepage2;        //  homepage2
	CBitmapPicture  m_pic;
// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
   
// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	afx_msg void OnDoubleclickedAuthor();
	afx_msg void OnAuthor();
	afx_msg void OnPostMoney();
	afx_msg void OnDeclare();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
	//virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	CString cs="���ߣ�����껣��У�1970��4������1998���ҵ�ڱ����ʵ��ѧ����ʿѧλ��רҵ���˹���������Ȼ���Դ����Ȱ���Ϸ�����Լ������ĵز���";
	CString cs1="";
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_GUO_EMAIL, m_urlEMail);
	DDX_Control(pDX, IDC_GUO_GROUPS, m_urlGroups);
	DDX_Control(pDX, IDC_LOGO, m_pic);

	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_BN_DOUBLECLICKED(IDC_AUTHOR, OnDoubleclickedAuthor)
	ON_BN_CLICKED(IDC_AUTHOR, OnAuthor)
	ON_BN_CLICKED(IDC_POST_MONEY, OnPostMoney)
	ON_BN_CLICKED(IDC_DECLARE, OnDeclare)
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// when user double click a small area near icon
void CAboutDlg::OnDoubleclickedAuthor() 
{
	// TODO: Add your control notification handler code here
	AfxMessageBox("Programmed by Guo xianghao(guoxh@hotmail.com)2000/12");
}

void CAboutDlg::OnAuthor() 
{
	// TODO: Add your control notification handler code here
	OnDoubleclickedAuthor() ;
}

void CAboutDlg::OnPostMoney() 
{
	// TODO: Add your control notification handler code here
	CPostMoneyDlg dlg;
	dlg.DoModal();
}

void CAboutDlg::OnDeclare() 
{
	// TODO: Add your control notification handler code here
	CShengmingDlg dlg;
	dlg.DoModal();
}
void CAboutDlg::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	static int i=0;
	CString cs1="���ߣ�����껣��У�1970��4������1998���ҵ�ڱ����ʵ��ѧ����ʿѧλ��רҵ���˹���������Ȼ���Դ���Ŀǰ������Ȼ���Դ����о�������������Ȱ���Ϸ�����Լ������ĵز����ڱ�����������10�ꡣ";
    CString cs2="�Ұ���Ȼ����������ѧ���������Ҫʹ��C/C++,���Ƕ�80x86�������Ҳ�ǳ���Ϥ��������Ů���ѣ��������Һ��ӵ��裩���һ��������������һ���ͨLISP��Prolog���ԣ�����Ϣ���������͸�����������к�������о���";
	i+=1;
	if(i>5)
	{
		if(i>10)
        {
	        AfxMessageBox(cs2);
			i=0;
		}
		else if(i==6)
			AfxMessageBox(cs1);
	}
	CDialog::OnRButtonDown(nFlags, point);
}

BOOL CAboutDlg::OnInitDialog() 
{

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_pic.SetBitmap(IDB_LOGO);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// Main user interface dialog :CSelectionDlg dialog

// init some(if not all) important varibles
CSelectionDlg::CSelectionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectionDlg::IDD, pParent)
{
	
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_City=BEIJING;
	m_pDragImage = NULL;
	m_pDragWnd = NULL;
	//init game
	int i;
	MyCash=2000;    // my init cash is 2000
	MyDebt=5000;    // my init debt is 5000
	MyBank=0;       // bank savings is 0
	myTotal=0;      // init goods I have
	myCoat=100;     // totally how many goods I can carry, due to house size
	m_nMyHealth=100; // init health points
	m_MyFame=100;    // init fame points
	m_nVisitWangba=0; // how many times a user can visit wangba, to avoid wanba abuse
	m_MyCurrentLoc=-1;   // I'm not in any of the locations at first
    // user can change the following varibles by set up dialog: 
	m_bCloseSound=FALSE; // sound is open at game begining by default
	m_bHackActs=FALSE;   // don't allow hacker's action in bank network by default 
    
	m_nTimeLeft=40;      //this is the total turns user can play
	
	/*---------------------- init   goods  ------------------------------------------*/
	for(i=0;i<8;i++){
		m_DrugPrice[i]=0;
		m_nMyDrugs[i]=-1;  // do not have any goods in the begining
	}
    for(i=0;i<9;i++)       // alloc memory for goods' name
		m_chDrugName[i]=(char *)malloc(100);
	
	// init the goods' name	
	strcpy(m_chDrugName[0],"��������");	
	strcpy(m_chDrugName[1],"��˽����");
	strcpy(m_chDrugName[2],"����VCD����Ϸ");	
	strcpy(m_chDrugName[3],"�ٰ׾ƣ��綾����");
	strcpy(m_chDrugName[4],"���Ϻ�С�����������飩");	
	strcpy(m_chDrugName[5],"�������");
	strcpy(m_chDrugName[7],"α�ӻ�ױƷ");	
	strcpy(m_chDrugName[6],"ˮ���ֻ�");
	strcpy(m_chDrugName[8],"");

   /*------------------- end of init goods -----------------------------------------*/
}

CSelectionDlg::~CSelectionDlg()
{
	delete m_pDragImage;
	m_pDragImage = NULL;
}

void CSelectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectionDlg)
	DDX_Control(pDX, IDC_LIST2, m_list2);
	DDX_Control(pDX, IDC_LIST1, m_list1);
	DDX_Control(pDX, IDC_MY_DEBT_LED, m_Text2);
	DDX_Control(pDX, IDC_STATIC_COUNTER, m_HealthDisplay);
	DDX_Control(pDX, IDC_STATIC_FAME, m_FameDisplay);
	DDX_Control(pDX, IDC_BANK_MONEY, m_BankDisplay);
	DDX_Control(pDX, IDC_CASH, m_CashDisplay);

	DDX_Control(pDX, IDC_STATIC1, m_sTicker);
	DDX_Control(pDX, IDC_BG, m_Picture);
	//DDX_Radio(pDX,IDC_LOC_BEIJINGZHAN,m_nLoc);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectionDlg, CDialog)
	//{{AFX_MSG_MAP(CSelectionDlg)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	ON_BN_CLICKED(IDC_ADDALL, OnAddAll)
	ON_BN_CLICKED(IDC_REMOVEALL, OnRemoveAll)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST2, OnDblclkList2)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST1, OnBegindragList1)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST2, OnBegindragList2)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, OnColumnclickList1)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST2, OnColumnclickList2)
	ON_BN_CLICKED(IDC_LOC_JIANGUOMEN, OnLocJianguomen)
	ON_BN_CLICKED(IDC_LOC_BEIJINGZHAN, OnLocBeijingzhan)
	ON_BN_CLICKED(IDC_LOC_XIZHIMEN, OnLocXizhimen)
	ON_BN_CLICKED(IDC_LOC_CHONGWENMEN, OnLocChongwenmen)
	ON_BN_CLICKED(IDC_LOC_DONGZHIMEN, OnLocDongzhimen)
	ON_BN_CLICKED(IDC_LOC_FUXINGMEN, OnLocFuxingmen)
	ON_BN_CLICKED(IDC_LOC_JISHUITAN, OnLocJishuitan)
	ON_BN_CLICKED(IDC_LOC_CHANGCHUNJIE, OnLocChangchunjie)
	ON_BN_CLICKED(IDC_GOTO_BANK, OnGotoBank)
	ON_BN_CLICKED(IDC_HOSPITAL, OnHospital)
	ON_BN_CLICKED(IDC_POSTOFFICE, OnPostoffice)
	ON_COMMAND(ID_EXIT_ME, OnExit)
	ON_COMMAND(ID_HIGH, OnHighScore)
	ON_COMMAND(IDM_BANK, OnBank)
	ON_COMMAND(IDM_HOSPIAL, OnMenuHospial)
	ON_COMMAND(IDM_AIRPORT, OnAirport)
	ON_COMMAND(IDM_WANGBA, OnWangba)
	ON_COMMAND(ID_INTRO, OnIntro)
	ON_COMMAND(ID_ABOUT, OnAbout)
	ON_BN_CLICKED(IDC_TRY, OnTry)
	ON_BN_CLICKED(IDC_POST, OnPost)
	ON_BN_CLICKED(IDC_AGENCY, OnHouseAgency)
	ON_COMMAND(IDM_NEW_GAME, OnNewGame)
	ON_COMMAND(IDM_HOUSE_AGENT, OnHouseAgent)
	ON_COMMAND(IDM_SETUP, OnSetup)
	ON_COMMAND(IDC_STORY, OnStory)
	ON_COMMAND(IDC_BEIJING_INTRO, OnBeijingIntro)
	ON_WM_KEYDOWN()
	ON_BN_CLICKED(IDC_BOSS, OnBoss)
	ON_BN_CLICKED(IDC_LOC_GONGZHUFEN, OnLocGongzhufen)
	ON_BN_CLICKED(IDC_LOC_PINGGUOYUAN, OnLocPingguoyuan)
	ON_COMMAND(IDM_POSTOFFICE, OnPostoffice)
	ON_BN_CLICKED(IDC_HOSP, OnHospital)
	ON_BN_CLICKED(IDC_WANGBA, OnWangba)
	ON_BN_CLICKED(IDC_GO_SHANGHAI, OnGoShanghai)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectionDlg message handlers.

// some other varibles are initialized.
BOOL CSelectionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
		//	pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// user can't close this dialog directly. ie, the close box on the top right is 
	// disbaled. This is to avoid user close the application directly, cause the help
	// html file not to be deleted.
	CMenu* mnu = this->GetSystemMenu(FALSE);
    mnu->ModifyMenu(SC_CLOSE,MF_BYCOMMAND | MF_GRAYED );
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
   
	// full row select
	m_list1.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	m_list2.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

	// set image list to listctrl
	if ( m_ImageList.Create(16, 16, ILC_COLOR, 0, 4) &&
	     m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_MONEY)) != -1)
	{
		m_list1.SetImageList(&m_ImageList, LVSIL_SMALL);
		m_list2.SetImageList(&m_ImageList, LVSIL_SMALL);
	}

	// create black market column
	m_list1.InsertColumn(0, "��Ʒ", LVCFMT_LEFT, 68);
	m_list1.InsertColumn(1, "���м۸�", LVCFMT_LEFT, 67);
	m_list1.InsertColumn(2, "", LVCFMT_LEFT, 27);
    // create house column
	m_list2.InsertColumn(0, "��Ʒ", LVCFMT_LEFT, 68);
	m_list2.InsertColumn(1, "����۸�", LVCFMT_LEFT, 67);
	m_list2.InsertColumn(2, "����", LVCFMT_LEFT, 57);

	// add some items
	for (int i = 0; i < 1; i++)
	{
		CString str;
		str.Format("(%03d , ", i);
		int n = m_list1.InsertItem(i, str + "01)", 0);
		m_list1.SetItemText(n, 1, str + "02)");
		m_list1.SetItemText(n, 2, str + "03)");
	}
    m_nBuyCount=0;
	m_nSellCount=0;
	m_nMyHealth=100;         // well , I'm fit
	m_MyFame=100;            // well, my fame is very OK
	m_nVisitWangba=0;
	m_nSortList1Item = -1;
	m_nSortList2Item = -1;
    m_list1.SetColumnWidth(0,150);
    m_list2.SetColumnWidth(0,100);
	m_hcArrow = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	m_hcNo = AfxGetApp()->LoadStandardCursor(IDC_NO);

    // the text color to display debt is red.
    m_Text2.SetBackColor(BLACK);
	m_Text2.SetColor(BLACK, RGB(240,0,0));
    // note: m_sTicker caused the game can not be run under Windows XP.
#ifndef FOR_WINDOWS_XP
	// load and display real time news on the bottom
	m_sTicker.SetTickerText(" Loading data, please wait...");
	//IMPORTANT CALL to set the Window Coordinates of the Ticker
	m_sTicker.GetWindowRect (&m_TickerRect);
	m_sTicker.ShowRates ("Ticker.txt", RGB(255,255,255),RGB(255,255,43),"Comic Sans MS",17,4, MODE_RATES_TICKER);
    // set the scrol speed of news
	m_sTicker.SetTickerSpeed (2);
#endif
	// 5 goods at start
	makeDrugPrices(3);
	HandleCashAndDebt();
	DisplayDrugs();

	//  cool  buttons
     // boss coming protect. if user press this button, a large dialog appears, cover the
     // game
	 m_btnBoss.SubclassDlgItem(IDC_BOSS, this);  
	 COLORREF crStandard = ::GetSysColor(COLOR_BTNFACE);
     m_btnBoss.SetInactiveBgColor(crStandard);
     m_btnBoss.SetActiveBgColor(crStandard); 
     
	 m_btnShanghai.SubclassDlgItem(IDC_GO_SHANGHAI, this);  
	 crStandard = ::GetSysColor(COLOR_BTNFACE);
     m_btnShanghai.SetInactiveBgColor(crStandard);
     m_btnShanghai.SetActiveBgColor(crStandard); 
    
     // bank botton 
	 m_btnTry.SubclassDlgItem(IDC_TRY, this);        // Assign the icon
	 m_btnTry.SetIcon(IDI_HAND5, IDI_HAND4);

	 crStandard = ::GetSysColor(COLOR_BTNFACE);
     m_btnTry.SetInactiveBgColor(crStandard - RGB(10,10,10));
     m_btnTry.SetActiveBgColor(crStandard + RGB(40,40,40));
     // hospital button  
     m_btnHosp.SubclassDlgItem(IDC_HOSP, this);        // Assign the icon
	 m_btnHosp.SetIcon(IDI_HAND3, IDI_HAND2);

	 crStandard = ::GetSysColor(COLOR_BTNFACE);
     m_btnHosp.SetInactiveBgColor(crStandard - RGB(10,10,10));
     m_btnHosp.SetActiveBgColor(crStandard + RGB(40,40,40)); 
     // post office button
	 m_btnPost.SubclassDlgItem(IDC_POST, this);        // Assign the icon
	 m_btnPost.SetIcon(IDI_HAND7, IDI_HAND6);

	 crStandard = ::GetSysColor(COLOR_BTNFACE);
     m_btnPost.SetInactiveBgColor(crStandard - RGB(10,10,10));
     m_btnPost.SetActiveBgColor(crStandard + RGB(40,40,40));
     // house agency button
     m_btnAgency.SubclassDlgItem(IDC_AGENCY, this);        // Assign the icon
	 m_btnAgency.SetIcon(IDI_HAND1,IDI_HOUSE1);

	 crStandard = ::GetSysColor(COLOR_BTNFACE);
     m_btnAgency.SetInactiveBgColor(crStandard - RGB(10,10,10));
     m_btnAgency.SetActiveBgColor(crStandard + RGB(40,40,40));
     // network club button
	 m_btnNet.SubclassDlgItem(IDC_WANGBA, this);        // Assign the icon
	 m_btnNet.SetIcon(IDI_HAND9, IDI_HAND8);

	 crStandard = ::GetSysColor(COLOR_BTNFACE);
     m_btnNet.SetInactiveBgColor(crStandard - RGB(10,10,10));
     m_btnNet.SetActiveBgColor(crStandard + RGB(40,40,40));
    /*LoadBitmap (IDB_GAME_BK);
	SetTransparent (TRUE);
	SetTransColor (RGB(255,0,255));
	SetStaticTransparent (TRUE);
	SetClickAnywhereMove (TRUE);*/
	// redisplay all the variables
    RefreshDisplay();
	CString str;
	// display days user stay in Beijing
	str.Format("��������(%d/40��)",40-m_nTimeLeft);
    // show this on the dialog title
	SetWindowText(str);
	GenerateRandomHelpFile();
	// load the bitmap background
	m_Picture.SetBitmap(IDB_BG);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectionDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == ID_ABOUT)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CSelectionDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CSelectionDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSelectionDlg::OnAbout() 
{
	// TODO: Add your control notification handler code here
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

// "Buy" button is pressed. Handle the buy event
void CSelectionDlg::OnAdd() 
{
	CBuyDlg  dlg;
	CString num,str;
	CString drug_name;
	long    price=0;


    POSITION pos = m_list1.GetFirstSelectedItemPosition();
	while (pos)   {      
		int nItem = m_list1.GetNextSelectedItem(pos);
		drug_name=m_list1.GetItemText(nItem, 0);
    	num=m_list1.GetItemText(nItem, 1);
	    price=atol(num);
     
	}
	if(price==0)  //user did not select any drugs
	{
       	CRijiDlg dlg(NULL,"�һ�û��ѡ����ʲô��Ʒ�ء�");
        dlg.DoModal();
       	RefreshDisplay();
		return ;
	}
	// user do not have enough money to buy even one selected goods
	if(MyCash< price){
		if(MyBank>0){
	    CRijiDlg dlg(NULL,"�������ֽ𲻹���ȥ�������Ǯ�ɡ�");
        dlg.DoModal();
		}
		else
		{
        CRijiDlg dlg(NULL,"�����ֽ𲻹���������û�д�զ����?");
        dlg.DoModal();
		}

        	RefreshDisplay();
		return ;
	}
	// decide the max number of drug user can buy
	if(myTotal+MyCash/price >myCoat )
        dlg.m_nMaxCount=MIN(myCoat-myTotal,MyCash/price);
	else
        dlg.m_nMaxCount=MyCash/price;
    dlg.max=dlg.m_nMaxCount;
	dlg.cash=MyCash;
	dlg.drug_name=drug_name;
	if(dlg.DoModal()==IDOK){   //user decide to buy
		if(m_bCloseSound==FALSE)
        PlaySound("sound\\buy.wav",NULL,SND_ASYNC );
	m_nBuyCount=dlg.m_nMaxCount;   //this is the acutal number user buy
    myTotal+=m_nBuyCount;  // coat occupied since goods come
	if(myTotal>myCoat)
		myTotal=myCoat;
	
	MyCash-=price*m_nBuyCount;  // cash decrease after user buy something
    // display the changed cash
    str.Format("%ld",MyCash);
	m_CashDisplay.SetText(str);
	// move goods
	MoveListItems(m_list1, m_list2);	
    }
    	RefreshDisplay();
}

// handle the "Sell" event
void CSelectionDlg::OnRemove() 
{
	MoveListItems(m_list2, m_list1);
}

// this function is never called, it is part of the orignal example
void CSelectionDlg::OnAddAll() 
{
	// Select all then action
	for (int i = 0; i < m_list1.GetItemCount(); i++)
		m_list1.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);

	OnAdd();
}

// this function is never called, it is part of the orignal example
void CSelectionDlg::OnRemoveAll() 
{
	// Select all then action
	for (int i = 0; i < m_list2.GetItemCount(); i++)
		m_list2.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);

	OnRemove();
}
// this function is never called, it is part of the orignal example
void CSelectionDlg::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnAdd();
	*pResult = 0;
}
// this function is never called, it is part of the orignal example

void CSelectionDlg::OnDblclkList2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnRemove();
	*pResult = 0;
}

void CSelectionDlg::MoveListItems(CListCtrl & pFromList, CListCtrl & pToList)
{
    static int  bad_fame1=0, bad_fame2=0;   // static var to display fame being bad msg
	if (pFromList.GetSelectedCount() <= 0)
		return; // no row selected to move

	// Unselect row at pToList
	int k = -1;
	while ((k = pToList.GetNextItem(k, LVNI_SELECTED)) != -1)
	{
		pToList.SetItemState(k, 0, LVIS_SELECTED);
	}

	
    if(pFromList==m_list1){  //buy
        int n=0;
		int right_has=0;
		CString right_drug;
		// firstly, get the left drug_name & price
		POSITION pos = m_list1.GetFirstSelectedItemPosition();
		int nItem = m_list1.GetNextSelectedItem(pos);
		CString drug_name=m_list1.GetItemText(nItem, 0);
        CString price= m_list1.GetItemText(nItem, 1);
		// secondly, to see the house.
		// if no such goods find, add it
		// else merge it
		for (int i = 0; i < m_list2.GetItemCount(); i++)
		{
			right_drug=m_list2.GetItemText(i,0);
			if(right_drug==drug_name)  //find such drug in the right
			{
				right_has=1;
				break;
			}
		}
		int i = m_list2.GetItemCount();
		//find, the displayed price shoud be avarage of old and new
        if(right_has==1)  
		{
           CString old_cnt,old;
		   int j;
		   int old_price;
		   old_cnt=m_list2.GetItemText(i, 2);
		   old=old_cnt;
		   old_price=atoi(m_list2.GetItemText(i, 1));
		   j=atoi(old_cnt);
		   j+=m_nBuyCount;
		   old_cnt.Format("%d",j);
           m_list2.SetItemText(i,2,old_cnt);
		   CString guo;
		   guo.Format("%d",(int)((atoi(price)*m_nBuyCount+old_price*atoi(old))/(j)));
           m_list2.SetItemText(i,1,guo);
            
		}
		// well, can hold some goods there
		else if(m_nBuyCount!=0){
        CString cnt;
        n=m_list2.InsertItem(0,drug_name);
		m_list2.SetItemText(n,1,m_list1.GetItemText(nItem, 1));
		cnt.Format("%d",m_nBuyCount);
	    m_list2.SetItemText(n,2,cnt);
 	   }
	    // no more room left for hold the goods
		else{
         CString msg;
		 msg.Format("�ÿ�ϧ!����ķ���̫С��ֻ�ܷ�%d����Ʒ�������ķ���?",myCoat);
		 CRijiDlg dlg(NULL,msg);
         dlg.DoModal();
         RefreshDisplay();
		}
	}     
 
	else  //sell
	{
		int n=0;
		int left_has=0;
		CString left_drug;
		// firstly, get the left drug_name & price
		POSITION pos = m_list2.GetFirstSelectedItemPosition();
		int nItem = m_list2.GetNextSelectedItem(pos);
		CString drug_name=m_list2.GetItemText(nItem, 0);
        CString count= m_list2.GetItemText(nItem, 2);
		// secondly, to see the left
		// if no such drug find, add it
		// else merge it
		for (int i = 0; i < m_list1.GetItemCount(); i++)
		{
			left_drug=m_list1.GetItemText(i,0);
			if(left_drug==drug_name)  //find such drug in the right
			{
				left_has=1;
				break;
			}
		}
		int i= m_list1.GetItemCount();
        if(left_has==1)   //find
		{
			/*
			   1. Ask user how many he wants to sell
			      get the count at first
			   2. if sell all
			           delete right item
				  else
				        decrease right item count
               3. count house room and user's cash  
			
			*/
			CSellDlg dlg;
			dlg.m_nMaxCount=atoi(count);
			dlg.max=atoi(count);
			dlg.m_strSellMsg.Format("����%d��",atoi(count));
            dlg.m_strSellMsg+=drug_name;
            dlg.m_strSellMsg+="�����������٣�";
			if(dlg.DoModal()==IDOK){
              if(m_bCloseSound==FALSE)
			  PlaySound("sound\\money.wav",NULL,SND_ASYNC );
	         m_nSellCount=dlg.m_nMaxCount;   //this is the acutal number user buy
             myTotal-=m_nSellCount;  // coat occupied since drug in
			 MyCash+=m_nSellCount*atoi(m_list1.GetItemText(i,1));
			 CString str;
		
			  str.Format("%ld",MyCash);
	          m_CashDisplay.SetText(str);
			 if(m_nSellCount==atoi(count))  // all sold out
                m_list2.DeleteItem(nItem);
             else                          // sell only a part
			 {
				 CString drug_left;
				 drug_left.Format("%d",atoi(count)-m_nSellCount);
                 m_list2.SetItemText(nItem,2,drug_left);
			 }
			 // some drug would decrease user's fame
             if(drug_name=="���Ϻ�С�����������飩" )
			 {
				 	CString fame;
					fame="";
					fame+="����";
					fame+=drug_name;
					fame+=",";
					fame+="��Ⱦ���,���������仵�˰�!";
					// if it is the first time for a user to sell such goods,
					// display some message to inform fame being bad
					if(bad_fame1==0){	
				    CRijiDlg dlg(NULL,fame);
                    dlg.DoModal();
                    bad_fame1=1;
                    }
					CString str;
					m_MyFame-=7;     // decrease by 7 point every time
				    if(m_MyFame<60){
					m_FameDisplay.SetBackColor(BLACK);
                 	m_FameDisplay.SetColor(BLACK, RGB(240,0,0));
                 	}
					if(m_MyFame<0) m_MyFame=0;  // no neg value allowed.
					str.Format("%ld",m_MyFame);
	                m_FameDisplay.SetText(str);

			}
			 // ok, same as above
			 else if(drug_name=="�ٰ׾ƣ��綾����" )
			 {
				 	CString fame;
					fame="";
					fame+="����";
					fame+=drug_name;
					fame+=",";
					fame+="Σ����ᣬ���������½���.";
					if(bad_fame2==0){	
				    CRijiDlg dlg(NULL,fame);
                    dlg.DoModal();
                    bad_fame2=1;
                    }
					CString str;
					m_MyFame-=10;
					 if(m_MyFame<60){
					m_FameDisplay.SetBackColor(BLACK);
                 	m_FameDisplay.SetColor(BLACK, RGB(240,0,0));
                 	}
					if(m_MyFame<0) m_MyFame=0;
					str.Format("%ld",m_MyFame);
	                m_FameDisplay.SetText(str);

			 }


			}
          		RefreshDisplay();	
            
		}
		
		else{  // no one is selling the selected goods in the black market
        CString cnt;
        cnt="Ŷ���·�û������������";
		cnt+=drug_name;
		cnt+="���⡣";
		CRijiDlg dlg(NULL,cnt);
        dlg.DoModal();
       	RefreshDisplay();
		}
	}
	pToList.EnsureVisible(0, TRUE);
	pToList.SetFocus();
}

void CSelectionDlg::OnBegindragList1(NMHDR* pNMHDR, LRESULT* pResult) 
{

	OnBegindrag(&m_list1, pNMHDR);
	*pResult = 0;
}

void CSelectionDlg::OnBegindragList2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnBegindrag(&m_list2, pNMHDR);
	*pResult = 0;
}

void CSelectionDlg::OnBegindrag(CListCtrl* pList, NMHDR* pNMHDR) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if (pList->GetSelectedCount() <= 0)
		return;  // No row selected

	POINT pt;

 	m_pDragImage = CreateDragImageEx(pList, &pt);

	if (m_pDragImage == NULL)
		return;

	m_pDragWnd = pList;
	CPoint ptStart = pNMListView->ptAction;
	ptStart -= pt;
	m_pDragImage->BeginDrag(0, ptStart);
	m_pDragImage->DragEnter(GetDesktopWindow(), pNMListView->ptAction);
	SetCapture();
}

void CSelectionDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	CDialog::OnMouseMove(nFlags, point);
	if (m_pDragImage && m_pDragWnd) // In Drag&Drop mode ?
	{
		CPoint ptDropPoint(point);
		ClientToScreen(&ptDropPoint);
		m_pDragImage->DragMove(ptDropPoint);
		CWnd* pDropWnd = CWnd::WindowFromPoint(ptDropPoint);
		SetCursor (((pDropWnd == &m_list1) || (pDropWnd == &m_list2)) ? m_hcArrow : m_hcNo);
	}
#ifndef FOR_WINDOWS_XP
	// if user's mouse is on the real time news, stop the news
	m_sTicker.GetWindowRect (&m_TickerRect);
	ClientToScreen (&point);
	TRACK_TICKER_MOUSEMOVE(m_sTicker, m_TickerRect);
    m_sTicker.ResumeTicker();
#endif

}

void CSelectionDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_pDragImage && m_pDragWnd) // In Drag&Drop mode ?
	{
		::ReleaseCapture();
		m_pDragImage->DragLeave(GetDesktopWindow());
		m_pDragImage->EndDrag();
		
		CPoint pt(point);
		ClientToScreen(&pt);
	 	CWnd* m_pDropWnd = WindowFromPoint(pt);
		
		if ((m_pDragWnd == &m_list1) && (m_pDropWnd == &m_list2))
			OnAdd();
		else if ((m_pDragWnd == &m_list2) && (m_pDropWnd == &m_list1))
			OnRemove();

		m_pDragImage->DeleteImageList();
		delete m_pDragImage;
		m_pDragImage = NULL;
		m_pDragWnd = NULL;
	}
	
	CDialog::OnLButtonUp(nFlags, point);
}

CImageList* CSelectionDlg::CreateDragImageEx(CListCtrl *pList, LPPOINT lpPoint)
{
	if (pList->GetSelectedCount() <= 0)
		return NULL; // no row selected

	CRect rectSingle;
	CRect rectComplete(0,0,0,0);

	// Determine List Control Client width size
	pList->GetClientRect(rectSingle);
	int nWidth  = rectSingle.Width();

	// Start and Stop index in view area
	int nIndex = pList->GetTopIndex() - 1;
	int nBottomIndex = pList->GetTopIndex() + pList->GetCountPerPage() - 1;
	if (nBottomIndex > (pList->GetItemCount() - 1))
		nBottomIndex = pList->GetItemCount() - 1;

	// Determine the size of the drag image (limite for rows visibled and Client width)
	while ((nIndex = pList->GetNextItem(nIndex, LVNI_SELECTED)) != -1)
	{
		if (nIndex > nBottomIndex)
			break; 

		pList->GetItemRect(nIndex, rectSingle, LVIR_BOUNDS);

		if (rectSingle.left < 0) 
			rectSingle.left = 0;

		if (rectSingle.right > nWidth)
			rectSingle.right = nWidth;

		rectComplete.UnionRect(rectComplete, rectSingle);
	}
		
	CClientDC dcClient(this);
	CDC dcMem;
	CBitmap Bitmap;

	if (!dcMem.CreateCompatibleDC(&dcClient))
		return NULL;

	if (!Bitmap.CreateCompatibleBitmap(&dcClient, rectComplete.Width(), rectComplete.Height()))
		return NULL;

	CBitmap *pOldMemDCBitmap = dcMem.SelectObject(&Bitmap);
	// Use green as mask color
 	dcMem.FillSolidRect(0, 0, rectComplete.Width(), rectComplete.Height(), RGB(0,255,0));

	// Paint each DragImage in the DC
	nIndex = pList->GetTopIndex() - 1;
	while ((nIndex = pList->GetNextItem(nIndex, LVNI_SELECTED)) != -1)
	{	
		if (nIndex > nBottomIndex)
			break;

		CPoint pt;
		CImageList* pSingleImageList = pList->CreateDragImage(nIndex, &pt);

		if (pSingleImageList)
		{
			pList->GetItemRect(nIndex, rectSingle, LVIR_BOUNDS);
			pSingleImageList->Draw( &dcMem, 
									0, 
									CPoint(rectSingle.left - rectComplete.left, 
									       rectSingle.top - rectComplete.top), 
									ILD_MASK);
			pSingleImageList->DeleteImageList();
			delete pSingleImageList;
		}
	}

 	dcMem.SelectObject(pOldMemDCBitmap);
	CImageList* pCompleteImageList = new CImageList;
	pCompleteImageList->Create(rectComplete.Width(), rectComplete.Height(), ILC_COLOR | ILC_MASK, 0, 1);
	pCompleteImageList->Add(&Bitmap, RGB(0, 255, 0)); // Green is used as mask color
	Bitmap.DeleteObject();

	if (lpPoint)
	{
		lpPoint->x = rectComplete.left;
		lpPoint->y = rectComplete.top;
	}

	return pCompleteImageList;
}


void CSelectionDlg::OnColumnclickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if (pNMListView->iSubItem == m_nSortList1Item)
	{
		m_bSortList1Asc = !m_bSortList1Asc;
	}
	else
	{
		m_nSortList1Item = pNMListView->iSubItem;
		m_bSortList1Asc = TRUE;
	}

	// Sort list
	CListCtrlSortClass sc(&m_list1, m_nSortList1Item);
	sc.Sort(m_bSortList1Asc, CListCtrlSortClass::SortDataType::dtSTRING);

	*pResult = 0;
}

void CSelectionDlg::OnColumnclickList2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if (pNMListView->iSubItem == m_nSortList2Item)
	{
		m_bSortList2Asc = !m_bSortList2Asc;
	}
	else
	{
		m_nSortList2Item = pNMListView->iSubItem;
		m_bSortList2Asc = TRUE;
	}

	// Sort list
	CListCtrlSortClass sc(&m_list2, m_nSortList2Item);
	sc.Sort(m_bSortList2Asc, CListCtrlSortClass::SortDataType::dtSTRING);

	*pResult = 0;
}

// return the name of the goods ID(0~9)
CString CSelectionDlg::GetDrugName(int id)
{
	CString str;
	if(id<0 || id>9) // while, this is an internal error, impossible
		AfxMessageBox("ID error.");
    str=m_chDrugName[id];
	return str;
 
}
// --------------------------------------------------------------------------------------
// generate the goods' price, this is one of the key commponet of the game.
// The basic price of the drug is decide by this:
//              price=basic_price+ random(X)                   (1)
// where, 
//              basic_price is the basic price of a goods.
//              X is the range of price's change space.
// leaveout is the number of how much goods is leave out. So, the actual number of
// goods generated is 8-leaveout
//---------------------------------------------------------------------------------------
void CSelectionDlg::makeDrugPrices(int leaveout)
{
    int i, j;

	m_DrugPrice [0] = 100 + RandomNum(350);
	m_DrugPrice [1] = 15000 + RandomNum(15000);
	m_DrugPrice [2] = 5 + RandomNum(50);
	m_DrugPrice [3] = 1000 + RandomNum(2500);
	m_DrugPrice [4] = 5000 + RandomNum(9000);
	m_DrugPrice [5] = 250 + RandomNum(600);
	m_DrugPrice [6] = 750 + RandomNum(750);
	m_DrugPrice [7] = 65 + RandomNum(180);

	for (i = 0; i < leaveout; i++)
	{
		j = RandomNum(8);
		m_DrugPrice [j] = 0;
	}
}

//---------------------------------------------------------------------------------------
//  The events handle procedure. These events are of commercial nature, which have influence
// on user's goods, for instince,  the price and amount of goods may change after the
// event happen. 
//
//
//---------------------------------------------------------------------------------------
void CSelectionDlg:: DoRandomStuff(void)
{
    int i;
	int addcount;
	int exist=0;
	for (i = 0; i < GAME_MESSAGE_COUNT; i++)
	{
		if (!(RandomNum(950) % gameMessages [i].freq))
		{
			// no such goods, try next
			if (m_DrugPrice [gameMessages [i].drug] == 0)
				continue;
		    //display the message of the event
			CNewsDlg  dlg(NULL,gameMessages [i].msg);
			dlg.DoModal();
			//well, this is the events that CunZhaung give user a moibe and ask for
			// 2500
			if(i==GAME_MESSAGE_COUNT-1)
			MyDebt+=2500;
			// price multibled
			if (gameMessages [i].plus > 0)
				m_DrugPrice [gameMessages [i].drug] *= gameMessages [i].plus;
			// price minused
			if (gameMessages [i].minus > 0)
				m_DrugPrice [gameMessages [i].drug] /= gameMessages [i].minus;
			// goods added to user	
			if(	gameMessages [i].add>0)
			{
			  if (gameMessages [i].add + myTotal > myCoat)
				addcount = myCoat - myTotal;
			  else
				addcount = gameMessages [i].add;
			  // user do not have enough house to hold added goods 	
			  if(addcount==0)
			  {
					 CString msg;
					 msg.Format("��ϧ!����ķ���̫С��ֻ�ܷ�%d����Ʒ��",myCoat);
				     CRijiDlg dlg(NULL,msg);
                     dlg.DoModal();
			  	     return ;
			  }	
			  // try to find if user already have the goods to be added
			  for (int j = 0; j < m_list2.GetItemCount(); j++)
		      {
  			    if(strcmp(m_list2.GetItemText(j,0),
			         m_chDrugName[gameMessages [i].drug])==0)
			    {
				 exist=1;
				 break;
			    }
		       }
			  int j = m_list2.GetItemCount();
		      // ok, user have the goods to be added 
		      if(exist)   //add to existing goods
		      {
		     	int new_cnt=atoi(m_list2.GetItemText(j,2))+addcount;
		     	CString str;
		     	str.Format("%d",new_cnt);
		     	 myTotal+=addcount;
		     	m_list2.SetItemText(j,2,str);
		      }
		      //else insert new goods to my house
		      else   
		     {
		     	 CString cnt;
		     	 int n=m_list2.InsertItem(0,m_chDrugName[gameMessages [i].drug]);
		     	 //price =0 since user pay no money for the goods
    		     m_list2.SetItemText(n,1,"0");   
   		         cnt.Format("%d",addcount);
             	 m_list2.SetItemText(n,2,cnt);
             	  myTotal+=addcount;
		     }
		    }


			
		}
	}
		RefreshDisplay();
}

// this is location user travel to
void CSelectionDlg::OnLocJianguomen() 
{
	// TODO: Add your control notification handler code here
	if(m_MyCurrentLoc!=1){
	if(m_bCloseSound==FALSE)
	  PlaySound("sound\\shutdoor.wav",NULL,SND_ASYNC );
	  m_MyCurrentLoc=1;
	  HandleNormalEvents();

	}

}

void CSelectionDlg::OnLocBeijingzhan() 
{
	// TODO: Add your control notification handler code here
	if(m_MyCurrentLoc!=2){
		if(m_bCloseSound==FALSE)
	  PlaySound("sound\\shutdoor.wav",NULL,SND_ASYNC );
	  m_MyCurrentLoc=2;
	  HandleNormalEvents();

	}
}



void CSelectionDlg::OnLocXizhimen() 
{
	// TODO: Add your control notification handler code here
		 if(m_MyCurrentLoc!=3){
			 if(m_bCloseSound==FALSE)
	  PlaySound("sound\\shutdoor.wav",NULL,SND_ASYNC );
	  m_MyCurrentLoc=3;
	  HandleNormalEvents();

	}
}
// display goods avaible in black market
void CSelectionDlg::DisplayDrugs()
{
    int i,j=0;

	m_list1.DeleteAllItems();
   
	for (i = 0; i < 8; i++)
	{
		if(m_DrugPrice[i]!=0){
		CString str;
		str=GetDrugName(i);
		int n = m_list1.InsertItem(j, str, 0);
		str.Format("%ld",m_DrugPrice[i]);
		m_list1.SetItemText(n, 1, str);
		m_list1.SetItemText(n, 2, "");
		j+=1;

		}
	

	}
}

void CSelectionDlg::OnLocChongwenmen() 
{
	// TODO: Add your control notification handler code here
		if(m_MyCurrentLoc!=4){
			if(m_bCloseSound==FALSE)
	  PlaySound("sound\\shutdoor.wav",NULL,SND_ASYNC );
	  m_MyCurrentLoc=4;
	  HandleNormalEvents();

	}
}

void CSelectionDlg::OnLocDongzhimen() 
{
	// TODO: Add your control notification handler code here
		  if(m_MyCurrentLoc!=5){
			  if(m_bCloseSound==FALSE)
	  PlaySound("sound\\shutdoor.wav",NULL,SND_ASYNC );
	  m_MyCurrentLoc=5;
	  HandleNormalEvents();

	}
}

void CSelectionDlg::OnLocFuxingmen() 
{
	// TODO: Add your control notification handler code here
		 if(m_MyCurrentLoc!=6){
			 if(m_bCloseSound==FALSE)
	  PlaySound("sound\\shutdoor.wav",NULL,SND_ASYNC );
	  m_MyCurrentLoc=6;
	  HandleNormalEvents();

	}
}

void CSelectionDlg::OnLocJishuitan() 
{
	// TODO: Add your control notification handler code here
		 if(m_MyCurrentLoc!=7){
			 if(m_bCloseSound==FALSE)
	  PlaySound("sound\\shutdoor.wav",NULL,SND_ASYNC );
	  m_MyCurrentLoc=7;
	  HandleNormalEvents();

	}

}


//this is the procedure for change user's debt and savings in bank
//
//
void CSelectionDlg::HandleCashAndDebt()
{
    CString str;
	MyDebt=MyDebt + (long)(MyDebt * 0.10);

	str.Format("%ld",MyDebt);
	m_Text2.SetText(str);
	MyBank=MyBank + (long)(MyBank * 0.01);

	 str.Format("%ld",MyCash);
	m_CashDisplay.SetText(str);

		str.Format("%d",MyBank);
	m_BankDisplay.SetText(str);

}

void CSelectionDlg::OnLocChangchunjie() 
{
	// TODO: Add your control notification handler code here
    if(m_MyCurrentLoc!=8){
		if(m_bCloseSound==FALSE)
	  PlaySound("sound\\shutdoor.wav",NULL,SND_ASYNC );
	  m_MyCurrentLoc=8;
	  HandleNormalEvents();

	}
}
//--------------------------------------------------------------------------------------- 
//  This is the core engine for the game. do such things:
//  1. generate goods price  and display them in the black market
//  2. handle user's cash and debt and bank savings 
//  3. generate commerical events, health events, money-decrease events
//  4. test if debt is too large.
//  5. decrease the days left
//  6. if it is the last day, sell out user's goods if user have something left.
//--------------------------------------------------------------------------------------
void CSelectionDlg::HandleNormalEvents()
{
	CString str;
	int i, goods_num;
	CString goods_name;
    // the last day, user shoud have all things sold, hence all goods appears in the
    // black market.
	if(m_nTimeLeft<=2)   
		makeDrugPrices(0);
	else
	  makeDrugPrices(3);
    // cash and debt events
	HandleCashAndDebt();
	// commerical events
	DoRandomStuff();
	// display generated goods
	DisplayDrugs();
	// health events
	DoRandomEvent();
	// money decrease events
	OnSteal();
	// debt is too large, to be pulished.
	if(MyDebt>100000){
	 CRijiDlg dlg(NULL,"��ǷǮ̫�࣬�峤��һȺ�������˰�һ��!");
         dlg.DoModal();
		m_nMyHealth-=30;
		str.Format("%d",m_nMyHealth);
		m_HealthDisplay.SetText(str);
		if(m_bCloseSound==FALSE)
		 PlaySound("sound\\kill.wav",NULL,SND_ASYNC );

    }
    // one day passed
	m_nTimeLeft--;
    str.Format("��������(%d/40��)",40-m_nTimeLeft);
    // show this on the dialog title
	SetWindowText(str);
	// only one day left. Ask user to sell all goods
	if(m_nTimeLeft==1)
	{

     CRijiDlg dlg(NULL,"������ؼ��磬���ȫ������������");
         dlg.DoModal();
	}
	// no day left. Should end the game
	if(m_nTimeLeft==0)
	{
     CRijiDlg dlg(NULL,"���Ѿ��ڱ���40���ˣ��û�ȥ���ȥ�ˡ�");
         dlg.DoModal();
     // force to sell all the things user left in case user foret to 
     // sell something or he left something on purpose to test this game.
     // step1 : to see if user have someting not sold
	  int things_left=0;
	  CString goods_left="ϵͳ��������ʣ�����: ";
	  for (i = 0; i < m_list2.GetItemCount(); i++)
		{
			goods_left+=m_list2.GetItemText(i,0);
		
			things_left+=1;
			
			
		}
	  if(things_left!=0)  // so, there's some goods found un-sold.
	  {
		 goods_left+="��";
         CRijiDlg dlg(NULL,goods_left);
         dlg.DoModal();
		 int j=0;
		 // sell the godds automatically
		 {
           for (i = 0; i < m_list2.GetItemCount(); i++)
		   {

			goods_name=m_list2.GetItemText(i,0);
			goods_num=atoi(m_list2.GetItemText(i,2));
			// check the right for price
			for(j=0; j< m_list1.GetItemCount(); j++)
			{
				// if find
				if(goods_name==m_list1.GetItemText(j,0))
				{
                    // sell the goods using current black market price
					MyCash+=goods_num*(atoi(m_list1.GetItemText(j,1)));
                    
					
				}
             
			}
			
		   }
           
		 }

	  }
	 // exit the game
    OnExit();
   }
	RefreshDisplay();
}

// -------------------------------------------------------------------------------------
// Handle the health events.
//-------------------------------------------------------------------------------------
void CSelectionDlg::DoRandomEvent()
{
	int i=0;
	CString str,str1;
	CString snd;
	// where the user is found out to pass out.
	CString loc[21]={
		"������",
			"����վ", "��ֱ��",
			"������","��ֱ��",
			"������", "��ˮ̶","������","������","ƻ��԰",
	    "������",
			"�� ׯ", "������",
			"������","��Ԫ����",
			"�Ľ��", "������·","�˻�Ӫ","��΢·","�˽ǵ���",			
			""
	};
	// detailed location
	CString coffee[30]={
		    "������","���̯��","��̯��","������̯��","����������","��������","Ů������",
			"�в�����","�绰ͤ��","����Ů����","���⳵��","С����","����Ժ��",
			"С��ͤ��","С�̳��ſ�","�񹤽���","��������̯��","�ݵ���","���߸˶���",
			"С������","��·��","���е���","���Ĺ�԰��","�������","��������վ��",
			"��;����վ��","��������Ϸ���Ա�","���繫˾ʬ���Ա�","��ƭ��֪�����Ա�",""
	};
	// the health events.
    for (i = 0; i < EVENT_CNT; i++)
	{
		if (!(RandomNum(1000) % random_event [i].freq))
		{
           str.Format("%s",random_event [i].msg);
		   str1.Format("���Ľ���������%d�㡣",random_event [i].hunt);
		   str+=str1;
		   snd="sound\\";
		   snd+=random_event [i].sound;
		   if(m_bCloseSound==FALSE)
		   PlaySound(snd,NULL,SND_ASYNC);
		    CRijiDlg dlg(NULL,str);
         dlg.DoModal();
           m_nMyHealth-=random_event [i].hunt;
		   break;
		}
	}
    
	


		str.Format("%d",m_nMyHealth);
	m_HealthDisplay.SetText(str);  // health critical
    // user's health is little than 80 and it is more than 3 days to end the game.
    // He needs medical care without permission. The money for medical care would be
    // added to his debt. 
	if(m_nMyHealth<85 && m_nTimeLeft>3 ){
          CString msg1,msg2,msg3;
		  int delay_day=1+RandomNum(2);
          msg1.Format("���ĵ��������̧��ҽԺ��ҽ����������%d�졣",delay_day);
		  msg2="���ڲ�ע������,�ұ��˷��ֻ�����";
		  msg2+=loc[10*(m_City-1)+m_MyCurrentLoc-1];
		  msg2+="������";
		  msg2+=coffee[RandomNum(29)];
		  msg2+="��";
	      int load=delay_day*(1000+RandomNum(8500));
		  msg3.Format("�峤����Ϊ�ҵ渶��סԺ����%dԪ��",load);
    
          CRijiDlg dlg(NULL,msg1,msg2,msg3);
          dlg.DoModal();
          MyDebt+=load;
          m_nMyHealth+=10;   // health is increased by 10 points.
		  if(m_nMyHealth>100)
             m_nMyHealth=100;
          m_nTimeLeft-=delay_day;
         return ;
	}
	if(m_nMyHealth<20 && m_nMyHealth >0){

  CRijiDlg dlg(NULL,"���Ľ���..����Σ��..��ȥҽ..");
         dlg.DoModal();
         return ;
	}
	if(m_nMyHealth<0){  // Opps! user dead.
		if(m_bCloseSound==FALSE)
	 PlaySound("sound\\death.wav",NULL,SND_ASYNC );
 CRijiDlg dlg(NULL,"�����ڽ�ͷ,����ռǱ���д�ţ�\"�������ҽ�����!\"");
         dlg.DoModal();
		OnCancel();
	 }
	 	RefreshDisplay();

}


// handle bank events
void CSelectionDlg::OnGotoBank() 
{
	// TODO: Add your control notification handler code here
	if(m_bCloseSound==FALSE)
			 PlaySound("sound\\opendoor.wav",NULL,SND_ASYNC );
	CEnterBank   dlg(NULL,this);
	dlg.DoModal();
	CString str;

    
	 str.Format("%ld",MyCash);
	m_CashDisplay.SetText(str);

		str.Format("%d",MyBank);
	m_BankDisplay.SetText(str);
		RefreshDisplay();
}

// handle hospital event
void CSelectionDlg::OnHospital() 
{
	// TODO: Add your control notification handler code here
	if(m_bCloseSound==FALSE)
	 PlaySound("sound\\opendoor.wav",NULL,SND_ASYNC );
    // user need medical care
	if(m_nMyHealth<100){
	CString str;
	str.Format("�����˵������֣������Ľ���������%d����Ҫ���Ƶĵ�����%d��",
		m_nMyHealth,100-m_nMyHealth);
	CHispital dlg(NULL,this);
	dlg.m_strMsg=str;
	if(dlg.DoModal()==IDOK)
	{
		if(dlg.m_nCurSel*3500 >MyCash){

		CRijiDlg dlg(NULL,"ҽ��˵����Ǯ������! �ܾ����ơ���");
         dlg.DoModal();
		}
		else  //ok
		{
            m_nMyHealth+=dlg.m_nCurSel;
			MyCash-=dlg.m_nCurSel*3500 ;
           
				str.Format("%d",m_nMyHealth);
		m_HealthDisplay.SetText(str);
	     
          
		 str.Format("%ld",MyCash);
	m_CashDisplay.SetText(str);
		}
	}
   }
    // user is ok, need not mediacl care at all. He must be mad.
	else{
	CRijiDlg dlg(NULL,"С��ʿЦ��������Ű���\"��磡�񾭿���߹Һ�.\"");
    dlg.DoModal();
}
	RefreshDisplay();
}

// handle post office events
void CSelectionDlg::OnPostoffice() 
{
	// TODO: Add your control notification handler code here
	if(m_bCloseSound==FALSE)
	PlaySound("sound\\opendoor.wav",NULL,SND_ASYNC );
	// user doesn't have debt 
	if(MyDebt==0 ){
		// too poor
		if(MyCash+MyBank<1000){

		CRijiDlg dlg(NULL,"�峤�ٺ�Ц��������ûǮ,���񾭲�!��");
    dlg.DoModal();
	}
	//  richer
		else if(MyCash+MyBank<100000 && MyCash+MyBank>1000){
		CRijiDlg dlg(NULL,"�峤������ͷ��\"�ֵ�,����֧Ԯ����1000Ԫ��\"");
    dlg.DoModal();
	}
	    //very rich
		else if(MyCash+MyBank<10000000 && MyCash+MyBank>100000){
		CRijiDlg dlg(NULL,"�峤�ڵ绰�г����Ϲ�:\"����!�������Ů���޸���.\"...");
    dlg.DoModal();
	}
	//so rich that I admire you
		else if(MyCash+MyBank>10000000){
	CRijiDlg dlg(NULL,"�峤�ڵ绰�г����¹�˵��\"����ֱ�����׵���\"");
    dlg.DoModal();
   }
    // may this happen? I think never.
    else
    {
    	CRijiDlg dlg(NULL,"�峤˵��\"����ũ�������˵ĵ䷶��\"");
        dlg.DoModal();
    	}

		RefreshDisplay();
		return ;
	}
	// has debt
	CReplayLoad  dlg(NULL,this);
	CString str;
	str.Format("�峤�ڵ绰��˵��\"��ţ����Ƿ��%dԪ���컹!\"",MyDebt);
	dlg.m_strMsg=str;
	if(dlg.DoModal()==IDOK){
		// do not have enough money to pay
		if(dlg.m_nMaxMoney>MyCash){
			CRijiDlg dlg(NULL,"�峤���ſ��̡�ѩ��ؤ������Ƭ����Ц�������㻹������?��");
    dlg.DoModal();
    	RefreshDisplay();
			return ;
		}
		//pay
		MyDebt-=dlg.m_nMaxMoney;
		MyCash-=dlg.m_nMaxMoney;
	
		str.Format("%ld",MyDebt);
	m_Text2.SetText(str);
        
	 str.Format("%ld",MyCash);
	m_CashDisplay.SetText(str);
}
	RefreshDisplay();

}
//actually, this is not in Network club. It's in Airport
void CSelectionDlg::OnNetwork() 
{
	// TODO: Add your control notification handler code here
	//ShellExecute(0 ,"open", "http://www.beijing.gov.cn/chinese/index.asp", NULL, NULL, SW_MAXIMIZE);
	if(m_bCloseSound==FALSE)
	PlaySound("sound\\airport.wav",NULL,SND_ASYNC );

	CNetworkClub  dlg;
	dlg.DoModal();
		RefreshDisplay();
}

// handle the events which decrease user's money
void CSelectionDlg::OnSteal() 
{
	int i=0;
	CString str,str1;
    for (i = 0; i < STEAL_EVENT_CNT; i++)
	{
		if (!(RandomNum(1000) % random_steal_event [i].freq))
		{
           str.Format("%s",random_steal_event [i].msg);
           // cash decrease
		   if(i!=4 && i!=5){
		   str1.Format("�������Ӽ�����%d%%��",random_steal_event [i].ratoi);
		   str+=str1;
		   CRijiDlg dlg(NULL,str);
           dlg.DoModal();
           MyCash=(long)((MyCash/100)*(100-random_steal_event[i].ratoi));
		}
		// banking savings decrease
		   else if(MyBank>0)// i==4, telcome
		   {
           str1.Format("���Ĵ�������%d%%��",random_steal_event [i].ratoi);
		   str+=str1;
		   str+="����ѽ!";
		   //AfxMessageBox(str);
		   CRijiDlg dlg(NULL,str);
           dlg.DoModal();
           MyBank=(long)((MyBank/100)*(100-random_steal_event[i].ratoi));
		   }
		   break;
		}
	}
    
	// a hacker event happens that cause user's savings to be mad.
	if(!(RandomNum(1000)%25) && m_bHackActs==TRUE)
	{
		long num;
		if(MyBank<1000)   // if the savings is too little, give up
		  ;
		else if(MyBank>100000 )
		{
			num=(long)(MyBank/(2+RandomNum(20)));   // 2 instead of 1, so not all the money
			                           // can be token by hacker.
			if(RandomNum(20)%3!=0){
            str.Format("�ڿ������������磬����޸����ݿ⣬�ҵĴ�������%Ld",num);
			CRijiDlg dlg(NULL,str);
            dlg.DoModal();
            MyBank-=num; 	
            }
			else{
             str.Format("�ڿ������������磬����޸����ݿ⣬�ҵĴ��������%Ld",num);
			CRijiDlg dlg(NULL,str);
            dlg.DoModal();
            MyBank+=num;
             
			}
		}
        else		
		{
			num=(long)(MyBank/(1+RandomNum(15)));
			str.Format("�ڿ������������磬����޸����ݿ⣬�ҵĴ��������%Ld",num);
			CRijiDlg dlg(NULL,str);
            dlg.DoModal();
            MyBank+=num; 
		}  
	}
	
	
	if(MyCash<0){
		MyCash=0;
		 CRijiDlg dlg(NULL,"�����ð��ˡ�");
    dlg.DoModal();
	 }
     
   
    str.Format("%ld",MyCash);
	m_CashDisplay.SetText(str);
	str.Format("%d",MyBank);
	m_BankDisplay.SetText(str);
	RefreshDisplay();
}

// diaplay all the varible once
void CSelectionDlg::RefreshDisplay()
{
    CString str;
	str.Format("%ld",MyDebt);
	m_Text2.SetText(str);
	str.Format("%d",m_nMyHealth);
	m_HealthDisplay.SetText(str);
	str.Format("%d",m_MyFame);
	if(m_MyFame<60){
	m_FameDisplay.SetBackColor(BLACK);
    m_FameDisplay.SetColor(BLACK, RGB(240,0,0));
	}
	else
	{
		m_FameDisplay.SetBackColor(BLACK);
        m_FameDisplay.SetColor(BLACK, RGB(0,240,0));
	}
	m_FameDisplay.SetText(str);
	str.Format("%d",MyCash);
	m_CashDisplay.SetText(str);    
	str.Format("%d",MyBank);
	m_BankDisplay.SetText(str);
	// display goods/house_size
	str.Format("%d/%d",myTotal,myCoat);
	 GetDlgItem(IDC_WUPING)->SetWindowText(str);
	

}
// exit the game
void CSelectionDlg::OnExit() 
{
	// TODO: Add your command handler code here
	CString str;
	// get user's score
	int m_nScore=MyCash+MyBank-MyDebt;
	if(m_nScore>0)
	{
	
      CString name;
	  CTopPlayerDlg  playerdlg;
	  playerdlg.InitHighScores();
	  // to see the range of user, returns 100 if user is not among the top 10
      int i=playerdlg.GetMyOrder(m_nScore);
	  // if i=9, then user is No:10
	  if(i!=100) {   // enter top 10
      CRichTop10  topdlg;
	  topdlg.m_Score=m_nScore;
	  topdlg.m_Order=i+1;
	  topdlg.DoModal();
	  playerdlg.InsertScore(topdlg.m_strName,m_nScore,m_nMyHealth,m_MyFame);
	  
	  }
	  else  // not enter top 10
	  {
 
		 str.Format("������Ǯ%ldԪ�����̫�٣�û�ܽ��븻��ǰ10�����´�Ŭ��Ŷ!",m_nScore);
		 CNewsDlg dlg(NULL,str);
         dlg.DoModal();
	  }
      playerdlg.DoModal();
	  if(m_nScore >10000000)     // score very high, suggest report it
      {
         str.Format("������Ǯ%ldԪ����Һܸߣ��������������߽��и������С�",m_nScore);
		 CNewsDlg dlg(NULL,str);
         dlg.DoModal();
	  }
	}
	else   // user's score is not bigger than zero
	{
	
		CString name="������";
		CString str;
		str+="��������Ϸ��������: ��ҡ�";
		str+=name;
		str+="���ڱ���û����Ǯ����ǲ�ͻؼҡ�";
		CNewsDlg dlg(NULL,str);
        dlg.DoModal();
 	    remove(m_strHelpFile);
	    OnOK();
		return ;
	}
	// if user's score is bigger than zero, ask if he wants to try the game again
	if(AfxMessageBox("��, ����һ����?",MB_YESNO | MB_ICONQUESTION)==IDYES){
    m_nTimeLeft=40;  // well, user have 40 days, re-start
	OnNewGame();
	}
	else{

	remove(m_strHelpFile);  //delete the help file
	OnOK();  //then, quit
	}
}

void CSelectionDlg::OnHighScore() 
{
	// TODO: Add your command handler code here
	CTopPlayerDlg  dlg;
	dlg.InitHighScores();
	dlg.DoModal();
}

void CSelectionDlg::OnBank() 
{
	// TODO: Add your command handler code here
	OnGotoBank();
}

void CSelectionDlg::OnMenuHospial() 
{
	// TODO: Add your command handler code here
	OnHospital();
}

void CSelectionDlg::OnAirport() 
{
	// TODO: Add your command handler code here
	OnNetwork();
}

// deal with network club(wangba) events
void CSelectionDlg::OnWangba() 
{
	// TODO: Add your command handler code here
	// user is allowed enter wangba 4 times only, avoid wangba abuse
	if(m_nVisitWangba > 2)
	{
    CRijiDlg dlg(NULL,"�峤�ų�����������������������죬��ȥ����������! ");
    dlg.DoModal();
	return ;
	}
	if(MyCash<15){
    CRijiDlg dlg(NULL,"��������������Ҫ��15Ԫ���Ǻǣ�ȡǮ������");
    dlg.DoModal();
	}
	else {
		m_nVisitWangba+=1;  // ok, you visit once
	CWangba  dlg;
	dlg.DoModal();
	int i=RandomNum(10);  //give user some money( not bigger than 10)
	MyCash+=1+i;
	CString s;
	s.Format("��л���Ÿĸ�����������! �����������������%dԪ���ٺ�!",i+1);
    CRijiDlg dlg1(NULL,s);
    dlg1.DoModal();
	RefreshDisplay();
	}
}

// Deal with html help file.
// if it is defined HELP_ENC , the function is to encrypt the orignal html file to a 
// file named "helpinfo".
// else, this function would open a file (which filename is m_strHelpFile, system
// decrypt from "helpinfo" at program start.
// pay attention to the following line.
//#define   HELP_ENC
void CSelectionDlg::OnIntro() 
{
	FILE *fp1, *fp2;
#ifdef HELP_ENC
   // encrpty the help information
   fp1=fopen("oldhelp","rb");
   fp2=fopen("helpinfo","wb");
   int i;
   if(fp1!=NULL){

      char *buf;
	  buf=(char *)malloc(_filelength(_fileno(fp1)));
	  fread(buf,_filelength(_fileno(fp1)),1,fp1);
	  for(i=0;i<_filelength(_fileno(fp1));i++){
		  buf[i]=buf[i]  ^ 0x52;
		  fprintf(fp2,"%c",buf[i]);
	  }
	  fclose(fp2);
  	  fclose(fp1);
  }	  
#else	 // decripty the help info and open it using IE
  
	HINSTANCE hRun = ShellExecute(GetParent()->GetSafeHwnd(), _T("open"), 
		m_strHelpFile, NULL, NULL, SW_SHOW);
    if ((int) hRun <= 32)
	{
		MessageBox("�Ҳ��������ļ�","����",MB_ICONERROR);
	}
	
#endif	
}

void CSelectionDlg::OnTry() 
{
	// TODO: Add your control notification handler code here
	OnGotoBank();
}

void CSelectionDlg::OnPost() 
{
	// TODO: Add your control notification handler code here
	OnPostoffice();
}

// handles house agency event.
void CSelectionDlg::OnHouseAgency() 
{
	// TODO: Add your control notification handler code here
	//your house is too large
	if(myCoat==140){
	    CRijiDlg dlg1(NULL,"�н�˵�����ķ��ӱȾֳ��Ļ���!���ⷿ?");
        dlg1.DoModal();
		return ;
	}
	// no enough money to rent house
	if(MyCash<30000)
	{
		CRijiDlg dlg1(NULL,"�н�˵����û�������ֽ�����ⷿ? һ������ȥ!");
        dlg1.DoModal();
		return ;
	}
	else {
	CHouseDlg  dlg;
	dlg.old_number=myCoat;
	dlg.money=MyCash;
	// user buy, decrease the money
	if(dlg.DoModal()==IDOK){
	if(MyCash<=30000)
		MyCash-=25000;
	else{
		MyCash=MyCash/2;
		MyCash-=2000;
	}
	// enlage the house by ten
	myCoat+=10;
	RefreshDisplay();
	CString str;
	str.Format("�ҵķ��ӿ��Է�%d����Ʒ��!���ǣ������н鹫˾ƭ����һЩǮ...",myCoat);
    CRijiDlg dlg1(NULL,str);
    dlg1.DoModal();
	}
	// user gets up
	else
	{
     CRijiDlg dlg1(NULL,"��! С��Ϊ��! ���ű����е��ⷿ�н鹫˾����ƭ��....");
     dlg1.DoModal();
	}
}
}


// begin a new game. if it is within 3 days, would not ask the user to confirm, else
// confirm first.
void CSelectionDlg::OnNewGame() 
{   
	if((40-m_nTimeLeft)>=3)
	{   // confirm
		if(AfxMessageBox("��������һ����Ϸ��Ҫ����������ʼ�µ���?",MB_YESNO   )==IDNO)
			return ;
	}
	// TODO: Add your command handler code here
    m_pDragImage = NULL;
	m_pDragWnd = NULL;
	//init game
	int i;
	MyCash=2000;
	MyDebt=5000;
	MyBank=0;
	myTotal=0;
	myCoat=100;   //totally how many I can have
	m_nMyHealth=100;
	m_MyFame=100;
	m_nVisitWangba=0;
	m_MyCurrentLoc=-1;
	m_bHackActs=FALSE;
		// 40 days game
	m_nTimeLeft=40;
	for(i=0;i<8;i++){
		m_DrugPrice[i]=0;
		m_nMyDrugs[i]=-1;  // do not have any drug in the begining
	}
    for(i=0;i<9;i++)
		m_chDrugName[i]=(char *)malloc(100);
	strcpy(m_chDrugName[0],"��������");	
	strcpy(m_chDrugName[1],"��˽����");
	strcpy(m_chDrugName[2],"����VCD����Ϸ");	
	strcpy(m_chDrugName[3],"�ٰ׾ƣ��綾����");
	strcpy(m_chDrugName[4],"���Ϻ�С�����������飩");	
	strcpy(m_chDrugName[5],"�������");
	strcpy(m_chDrugName[7],"α�ӻ�ױƷ");	
	strcpy(m_chDrugName[6],"ˮ���ֻ�");
	strcpy(m_chDrugName[8],"");
	
    
	// add some items
	for ( i = 0; i < 1; i++)
	{
		CString str;
		str.Format("(%03d , ", i);
		int n = m_list1.InsertItem(i, str + "01)", 0);
		m_list1.SetItemText(n, 1, str + "02)");
		m_list1.SetItemText(n, 2, str + "03)");
	}
	// delete all the items in case user have old goods 
	m_list2.DeleteAllItems( );

    m_nBuyCount=0;
	m_nSellCount=0;
	m_nMyHealth=100;
	m_MyFame=100;
	m_nSortList1Item = -1;
	m_nSortList2Item = -1;
    m_list1.SetColumnWidth(0,150);
    m_list2.SetColumnWidth(0,100);

    CString str;
	str.Format("��������(%d/40��)",40-m_nTimeLeft);
    // show this on the dialog title
	SetWindowText(str);
	
	makeDrugPrices(3);
	HandleCashAndDebt();
	DisplayDrugs();
	RefreshDisplay();

}

//
//   Generate a file name for the html file. It is put on the same directory as the
//  execute file. now, the filename is "xxx/help.html"
//
void CSelectionDlg::GenerateRandomHelpFile()
{
  CString str=AfxGetApp()->m_pszHelpFilePath;
   int n=str.ReverseFind('\\');
   CString str1=str.Left(n);
   m_strHelpFile=str1;
  /* str1+="\\helpinfo";
  FILE *fp=fopen(str1,"rb");
  if(!fp){
	  AfxMessageBox(str1);
	  exit(-1);
  }
  int j=RandomNum(100);*/
  CString tmp;
  tmp.Format("help.html");
  m_strHelpFile+="\\";
  m_strHelpFile+=tmp;
  /*FILE *fp1=fopen(m_strHelpFile,"wb");
   int i;
   if(fp!=NULL){
	  char *buf;
	  buf=(char *)malloc(_filelength(_fileno(fp)));
	  fread(buf,_filelength(_fileno(fp)),1,fp);
	  for(i=0;i<_filelength(_fileno(fp));i++)
		  buf[i]=buf[i]  ^ 0x52;
	  fwrite(buf,_filelength(_fileno(fp)),1,fp1);
	  free(buf);
	fclose(fp);
	fclose(fp1);
	}*/
}

void CSelectionDlg::OnHouseAgent() 
{
	// TODO: Add your command handler code here
	OnHouseAgency();
}
// -------------------------------------------------------------------------------------
// game setup. 
// -------------------------------------------------------------------------------------
void CSelectionDlg::OnSetup() 
{
	// TODO: Add your command handler code here
	CSetUpGame  dlg;
	dlg.m_bCloseSound=m_bCloseSound;
	dlg.m_bHackerActivity=m_bHackActs;
	if(dlg.DoModal()==IDOK)
    {
		m_bCloseSound=dlg.m_bCloseSound;
		m_bHackActs=dlg.m_bHackerActivity;
	}
}

// -------------------------------------------------------------------------------------
// Show the game story
// -------------------------------------------------------------------------------------
void CSelectionDlg::OnStory() 
{
	// TODO: Add your command handler code here
	CStoryDlg dlg(NULL,1);
	dlg.DoModal();
}

//-
//  show Beijing introducing
//-
void CSelectionDlg::OnBeijingIntro() 
{
	// TODO: Add your command handler code here
   CBeijingIntroDlg   dlg;
   dlg.DoModal();
}

// -------------------------------------------------------------------------------------
//    Key down events. Not handled actually
// -------------------------------------------------------------------------------------
void CSelectionDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	static int num=0;
	num+=1;
	if(num>12)
	{
		AfxMessageBox("You pressed the keyborad 12 times.");
		num=0;
	}
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

// ------------------------------------------------------------------------------------
//   Boss protection. Enable a large dialog which would cover the whole game.
//   The dialog enabled has a edit box, filled with some words related to "how to
//   make my work better.", well, this is nothing but a joke, hehe.
// ------------------------------------------------------------------------------------
void CSelectionDlg::OnBoss() 
{
	// TODO: Add your control notification handler code here
	CBossComeDlg dlg;
	dlg.DoModal();
}

void CSelectionDlg::OnLocGongzhufen() 
{
	// TODO: Add your control notification handler code here
	 if(m_MyCurrentLoc!=9){
			 if(m_bCloseSound==FALSE)
	  PlaySound("sound\\shutdoor.wav",NULL,SND_ASYNC );
  m_MyCurrentLoc=9;
	  HandleNormalEvents();
	
	}

}

void CSelectionDlg::OnLocPingguoyuan() 
{
	// TODO: Add your control notification handler code here
	 if(m_MyCurrentLoc!=10){
			 if(m_bCloseSound==FALSE)
	  PlaySound("sound\\shutdoor.wav",NULL,SND_ASYNC );
	  m_MyCurrentLoc=10;
	  HandleNormalEvents();

	}
	 
	 

}

void CSelectionDlg::OnGoShanghai() 
{
	// TODO: Add your control notification handler code here
   switch(m_City)
   {
   case BEIJING:   //user is in Beijing, so he wants to go to shanghai
	   m_City=SHANGHAI;
      GetDlgItem(IDC_LOC)->SetWindowText("�����е���ʾ��ͼ");
       GetDlgItem(IDC_LOC_XIZHIMEN)->SetWindowText("������");
       GetDlgItem(IDC_LOC_FUXINGMEN)->SetWindowText("���ҽ�");
       GetDlgItem(IDC_LOC_JISHUITAN)->SetWindowText("���˴�");
       GetDlgItem(IDC_LOC_DONGZHIMEN)->SetWindowText("��Ԫ����");
       GetDlgItem(IDC_LOC_JIANGUOMEN)->SetWindowText("������");
       GetDlgItem(IDC_LOC_BEIJINGZHAN)->SetWindowText("�� ׯ");
       GetDlgItem(IDC_LOC_CHONGWENMEN)->SetWindowText("������");
       GetDlgItem(IDC_LOC_CHANGCHUNJIE)->SetWindowText("��ȪӪ");
       GetDlgItem(IDC_LOC_GONGZHUFEN)->SetWindowText("��΢·");
       GetDlgItem(IDC_LOC_PINGGUOYUAN)->SetWindowText("�˽���·");
	   GetDlgItem(IDC_GO_SHANGHAI)->SetWindowText("��Ҫ������");
	   break;
   case SHANGHAI: //user is in shanghai, he wants to go back to beijing
       m_City=BEIJING;
       //GetDlgItem(IDC_BG)->EnableWindow(FALSE);
       GetDlgItem(IDC_LOC)->SetWindowText("�����е���ʾ��ͼ");
       GetDlgItem(IDC_LOC_XIZHIMEN)->SetWindowText("��ֱ��");
       GetDlgItem(IDC_LOC_FUXINGMEN)->SetWindowText("������");
       GetDlgItem(IDC_LOC_JISHUITAN)->SetWindowText("��ˮ̶");
       GetDlgItem(IDC_LOC_DONGZHIMEN)->SetWindowText("��ֱ��");
       GetDlgItem(IDC_LOC_JIANGUOMEN)->SetWindowText("������");
       GetDlgItem(IDC_LOC_BEIJINGZHAN)->SetWindowText("����վ");
       GetDlgItem(IDC_LOC_CHONGWENMEN)->SetWindowText("������");
       GetDlgItem(IDC_LOC_CHANGCHUNJIE)->SetWindowText("������");
       GetDlgItem(IDC_LOC_GONGZHUFEN)->SetWindowText("������");
       GetDlgItem(IDC_LOC_PINGGUOYUAN)->SetWindowText("ƻ��԰");
	   GetDlgItem(IDC_GO_SHANGHAI)->SetWindowText("��Ҫ�侩��");
	   break;
   default:
	   break;
   }
    

  
}
