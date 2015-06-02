/*
 *  TOPPERS ECHONET Lite Communication Middleware
 * 
 *  Copyright (C) 2015 Cores Co., Ltd. Japan
 * 
 *  上記著作権者は，以下の(1)～(4)の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *      の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *      と．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *        報告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *      免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *  の責任を負わない．
 * 
 *  @(#) $Id$
 */

using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.Html;
using System.Linq;
using System.Net.WebSockets;
using System.Serialization;
using System.Text;
using System.Text.RegularExpressions;
using System.Web;
using System.Xml;
using control;
using ctrlui;
using jQueryApi;
using Kadecot;

public class CtrlUI
{
	public static CtrlUI Page;
	public static Information Info;

	public static void Main()
	{
		if (Page == null) {
			Page = new CtrlUI();
			Info = new Information();
		}

		jQuery.Document.On("pageinit", "#index", Page.InitClassGroups);
	}

	private static Element page;
	private Dictionary<DeviceController.DeviceInfo, DeviceController.DeviceInfo> Read = new Dictionary<DeviceController.DeviceInfo, DeviceController.DeviceInfo>();

	public static Element GetPage(Element ele)
	{
		var page = ele;
		if (jQuery.Select(page).Is("[data-role=\"page\"]")) {
			return page;
		}
		if (jQuery.Select(page).Parents("[data-role=\"page\"]").Length == 1) {
			return jQuery.Select(page).Parents("[data-role=\"page\"]").GetElement(0);
		}
		return null;
	}

	/// <summary>クラスグループ（ClassGroupList.json）</summary>
	static JsonClassGroupInfo[] m_ClassGroups;

	/// <summary>クラス</summary>
	static JsonClassInfo m_ProfileClassInfo;
	static List<JsonClassInfo> m_Classes;

	WampClient m_WampClient;
	/// <summary>現在接続中の機器</summary>
	DeviceController m_DeviceController;

	private DeviceController.DeviceInfo _m_CurrentNode;

	/// <summary>現在選択中のノード</summary>
	DeviceController.DeviceInfo m_CurrentNode
	{
		get		{			return _m_CurrentNode;		}
		set
		{
			bool flag = (_m_CurrentNode != value) || (_m_CurrentNode == null && value == null);
			_m_CurrentNode = value;

			if (flag)
				jQuery.Document.Trigger(UIEventNames.CurrentNodeChange);
		}
	}

	/// <summary>
	/// コンストラクタ
	/// </summary>
	public CtrlUI()
	{
		m_WampClient = new WampClient();
		m_WampClient.addOpenCallback(WampClientConnected);
		m_WampClient.addCloseCallback(WampClientDisconnected);
		m_WampClient.connect("ws://" + Window.Location.Host + "/webapi.ashx");
	}

	/// <summary>
	/// #indexページ読み込み時に実行
	/// </summary>
	/// <param name="ele"></param>
	/// <param name="ev"></param>
	private void InitClassGroups(jQueryEvent eve)
	{
		page = eve.Target;
		SetEvent();

		if (m_ClassGroups != null)
			return;

		DeviceController.GetClassInfo = GetClassInfoHandler;
		m_CurrentNode = null;
		string ClassGroupListUrl = "devices/ClassGroupList.json";
		jQuery.Ajax(ClassGroupListUrl).Success(InitClassGroups).Error(
			(request, textStatus, error) => {
				AjaxError(request, textStatus, error, ClassGroupListUrl);
			});
	}

	private void SetEvent()
	{
		jQuery.Document.On(UIEventNames.LoadingShow, UIEventHandler.LoadingShow);
		jQuery.Document.On(UIEventNames.LoadingHide, UIEventHandler.LoadingHide);
		jQuery.Document.On(UIEventNames.CurrentNodeChange, this.CurrentNodeChange);
		jQuery.Document.On(UIEventNames.SearchStart, this.SearchStart);
		jQuery.Document.On(UIEventNames.SearchEnd, this.SearchEnd);
		jQuery.Document.On(UIEventNames.SearchError, this.SearchError);
		jQuery.Document.On(UIEventNames.PropertyWriteStart, this.PropertyWriteStart);
		jQuery.Document.On(UIEventNames.PropertyWriteEnd, this.PropertyWriteEnd);
		ConnectionSwt.Change(Connect);
		SetPropertyBtn.Click(SetProperty);
		GetPropertyBtn.Click(GetProperty);
		SetGetPropertyBtn.Click(SetGetProperty);
		KadecotBtn.Click(KadecotClick);
		PropertyList.Change(PropertyChange);
		SetInitParamsBtn.Click(SetInitParams);
	}

	/// <summary>
	/// "devices/ClassGroupList.json"ファイルの読み込みに成功したとき実行します
	/// </summary>
	/// <param name="data">ファイルのデータ</param>
	private void InitClassGroups(object data)
	{
		List<JsonClassGroupInfo> classGroups = data.ToJSClassGroupInfoList();
		//初期化
		m_Classes = new List<JsonClassInfo>();
		m_ClassGroups = classGroups.ToArray();
		GetClassInfoHandler(T_ECN_EOJ.X1_PROFILE, T_ECN_EOJ.X2_NODE_PROFILE, GetClassInfoCallback);
	}

	/// <summary>
	/// コールバック関数
	/// </summary>
	/// <param name="di"></param>
	private void GetClassInfoCallback(JsonClassInfo di)
	{
		m_ProfileClassInfo = di;
	}

	/// <summary>
	/// devices/ClassListxx.jsonファイルの読み込み
	/// </summary>
	/// <param name="x1">クラスグループコード</param>
	/// <param name="x2">クラスコード</param>
	/// <param name="cb">コールバック関数</param>
	private void GetClassInfoHandler(byte x1, byte x2, DeviceController.GetClassInfoCallback cb)
	{
		JsonClassInfo eclass = m_Classes.FirstOrDefault(p => (p.classGroup.classGroupCode == x1) && (p.classCode == x2));

		//すでにjsonファイルが読み込まれているので、ファイルを読み込まない。
		if (eclass != null) {
			if (eclass.properties == null)
				GetPropertyList(eclass, cb);
			else
				cb(eclass);
			return;
		}
		//eclass==nullの場合
		JsonClassGroupInfo classGroup = m_ClassGroups.FirstOrDefault(p => (p.classGroupCode == x1));

		string classListUrl = "devices/ClassList" + x1.ToString("X2").ToUpper() + ".json";

		jQuery.Ajax(classListUrl).Success((data) => {
			Debug.WriteLine(classListUrl + "読み込み成功");
			GetClassInfoSuccess(data, x1, x2, classGroup, cb);
		}).Error((request, textStatus, error) => {
			Debug.WriteLine(classListUrl + "読み込み失敗");
			GetClassInfoError(request, textStatus, error, eclass, cb, classListUrl);
		});
	}

	/// <summary>
	/// devices/ClassListxx.jsonファイルの取得に成功した時実行します。
	/// </summary>
	/// <param name="data"></param>
	private void GetClassInfoSuccess(object data, byte x1, byte x2, JsonClassGroupInfo classGroup, DeviceController.GetClassInfoCallback cb)
	{
		List<JsonClassInfo> classes = data.ToJSClassInfoList(classGroup);
		m_Classes.AddRange(classes);
		JsonClassInfo eclass = m_Classes.FirstOrDefault(p => (p.classGroup.classGroupCode == x1) && (p.classCode == x2));
		GetPropertyList(eclass, cb);
	}

	/// <summary>
	/// devices/PropertyListxxxx.jsonファイルの取得
	/// </summary>
	/// <param name="data"></param>
	private void GetPropertyList(JsonClassInfo eclass, DeviceController.GetClassInfoCallback cb)
	{
		string PropertyListUrl = "devices/PropertyList" + eclass.classGroup.classGroupCode.ToString("X2").ToUpper() + eclass.classCode.ToString("X2").ToUpper() + ".json";
		Debug.WriteLine(PropertyListUrl + "読み込み");

		jQuery.Ajax(PropertyListUrl).Success((data1) => {
			Debug.WriteLine(PropertyListUrl + "読み込み成功");
			GetPropertyListSuccess(data1, eclass);
		}).Error((request, textStatus, error) => {
			Debug.WriteLine(PropertyListUrl + "読み込み失敗");
			PropertyListAjaxError(request, textStatus, error, eclass, cb, PropertyListUrl);
		}).Complete((request, textStatus) => {
			GetPropertyListComplete(request, textStatus, eclass, cb);
		});
	}

	/// <summary>
	/// devices/PropertyListxxxx.jsonファイルの取得に成功
	/// </summary>
	/// <param name="data"></param>
	private void GetPropertyListSuccess(object data, JsonClassInfo eclass)
	{
		List<JsonPropertyInfo> properties = data.ToJSPropertyInfoList();
		eclass.properties = properties.ToArray();
	}

	/// <summary>
	/// devices/PropertyListxxxx.jsonファイルの取得後の処理
	/// </summary>
	/// <param name="request"></param>
	/// <param name="textStatus"></param>
	private void GetPropertyListComplete(jQueryXmlHttpRequest request, string textStatus, JsonClassInfo eclass, DeviceController.GetClassInfoCallback cb)
	{
		if (request.StatusText.Equals("OK")) {
			cb(eclass);
		}
	}

	/// <summary>
	/// devices/ClassListxx.jsonファイルがなかった場合
	/// </summary>
	/// <param name="request"></param>
	/// <param name="textStatus"></param>
	/// <param name="error"></param>
	private void GetClassInfoError(jQueryXmlHttpRequest request, string textStatus, Exception error,
		JsonClassInfo eclass, DeviceController.GetClassInfoCallback cb, string url)
	{
		AjaxError(request, textStatus, error, url);
		cb(eclass);
	}

	/// <summary>
	/// PropertyListxxxx.jsonファイル読み込みエラー
	/// </summary>
	/// <param name="request"></param>
	/// <param name="textStatus"></param>
	/// <param name="error"></param>
	private void PropertyListAjaxError(jQueryXmlHttpRequest request, string textStatus, dynamic error, JsonClassInfo eclass, DeviceController.GetClassInfoCallback cb, string url)
	{
		string PropertyListBaseUrl = "devices/PropertyListBase.json";
		jQuery.Ajax(PropertyListBaseUrl).Success((data1) => {
			Debug.WriteLine(PropertyListBaseUrl + "読み込み成功");
			GetPropertyListSuccess(data1, eclass);
		}).Error((request1, textStatus1, error1) => {
			Debug.WriteLine(PropertyListBaseUrl + "読み込み失敗");
			AjaxError(request1, textStatus1, error1, url);
		}).Complete((request1, textStatus1) => {
			GetPropertyListComplete(request1, textStatus1, eclass, cb);
		});
	}

	/// <summary>
	/// 
	/// </summary>
	private void sendHello()
	{
		m_WampClient.sendHello("default", "{\"roles\":{\"caller\":{},\"subscriber\":{}}}", OnWelcom);
	}

	private void OnWelcom(object args)
	{
		m_WampClient.sendSubscribe("{}", "com.sonycsl.kadecot.echonetlite.topic.GeneralLighting.OperationStatus", OnSubscribed, OnSubscribed);
	}

	private void OnSubscribed(object args)
	{
	}

	/// <summary>
	/// 機器検索ボタンを押下したとき実行
	/// </summary>
	/// <param name="args"></param>
	private void OnDevListed(object args)
	{
		int cmd = (int)((dynamic[])args)[0];
		if (cmd != (int)WAMP_MSG_TYPE.RESULT) {
			jQuery.Document.Trigger(UIEventNames.SearchError);
			return;
		}

		//初期化
		List<WampApiKadecotDevice> alldevs = args.GetAllDevices();
		GetDeviceInfo(alldevs, 0, new List<DeviceController.NodeInfo>());
	}

	/// <summary>
	/// 接続されている機器の設定値を取得
	/// </summary>
	/// <param name="alldevs"></param>
	/// <param name="pos"></param>
	private void GetDeviceInfo(List<WampApiKadecotDevice> alldevs, int pos, List<DeviceController.NodeInfo> m_NodeList)
	{
		WampApiKadecotDevice device = alldevs[pos];

		WampClientCore.WampCallbackArgs callback = (args) => {
			int cmd = (int)((dynamic[])args)[0];
			if (cmd != (int)WAMP_MSG_TYPE.RESULT) {
				jQuery.Document.Trigger(UIEventNames.SearchError);
				return;
			}

			WebApiObjectInfo dev = new WebApiObjectInfo(device.deviceId, ((dynamic[])args)[4], false);

			GetClassInfoHandler(dev.x1, dev.x2, (dc) => {
				DeviceController.DeviceInfo di = dc.GetDeviceInfo(dev, device);
				DeviceController.NodeInfo node = m_NodeList.FirstOrDefault((ni) => { return (string)ni.Data == device.ip_addr; });
				if (node == null) {
					var profile = new DeviceController.DeviceInfo(m_ProfileClassInfo, true, T_ECN_EOJ.X1_PROFILE, T_ECN_EOJ.X2_NODE_PROFILE, 1, dev.enodid);
					node = new DeviceController.NodeInfo((ENOD_ID)dev.enodid, profile);
					node.Data = device.ip_addr;
					m_NodeList.Add(node);
				}
				node.Devices.Add(di);

				pos++;
				if (pos < alldevs.Count) {
					GetDeviceInfo(alldevs, pos, m_NodeList);
				}
				else {
					jQuery.Document.Trigger(UIEventNames.SearchEnd);
					m_DeviceController = new DeviceController(m_NodeList);
					UpdateUI();
				}
			});
		};

		if (device.protocol == "arduino") {
			var arg = new
			{
				x1 = 0x0E,
				x2 = 0xF0,
				x3 = 0x01,
				enodid = 0x01,
				properties = new[] {
					new{epc = 0x00, flag = "RULE_SET,RULE_GET", size = 0x01},
					new{epc = 0x01, flag = "RULE_SET,RULE_GET", size = 0x01},
					new{epc = 0x02, flag = "RULE_SET,RULE_GET", size = 0x01},
					new{epc = 0x03, flag = "RULE_SET,RULE_GET", size = 0x01},
					new{epc = 0x04, flag = "RULE_SET,RULE_GET", size = 0x01},
					new{epc = 0x05, flag = "RULE_SET,RULE_GET", size = 0x01},
					new{epc = 0x06, flag = "RULE_SET,RULE_GET", size = 0x01},
					new{epc = 0x07, flag = "RULE_SET,RULE_GET", size = 0x01},
					new{epc = 0x08, flag = "RULE_SET,RULE_GET", size = 0x01},
					new{epc = 0x09, flag = "RULE_SET,RULE_GET", size = 0x01},
					new{epc = 0x0A, flag = "RULE_SET,RULE_GET", size = 0x01},
					new{epc = 0x0B, flag = "RULE_SET,RULE_GET", size = 0x01},
					new{epc = 0x0C, flag = "RULE_SET,RULE_GET", size = 0x01},
					new{epc = 0x0D, flag = "RULE_SET,RULE_GET", size = 0x01},
					new{epc = 0x0E, flag = "RULE_GET", size = 0x01},
					new{epc = 0x0F, flag = "RULE_GET", size = 0x01},
					new{epc = 0x10, flag = "RULE_GET", size = 0x01},
					new{epc = 0x11, flag = "RULE_GET", size = 0x01},
					new{epc = 0x12, flag = "RULE_GET", size = 0x01},
					new{epc = 0x13, flag = "RULE_GET", size = 0x01},
				}
			};
			callback(new object[] { 50, 0, new { deviceId = 1 }, new object[0], arg });
		}
		else {
			m_WampClient.sendCall("{\"deviceId\":" + device.deviceId + "}", "jp.toppers.ecnl.procedure.getDeviceInfo", null, null, callback);
		}
	}

	/// <summary>
	/// ノードリストを更新
	/// </summary>
	private void UpdateUI()
	{
		if (NodeList == null)
			return;
		NodeList.Html("");
		NodeList.Append(m_DeviceController.GetUINodeList(listitem_Click));
		NodeList.CollapsibleSet().CollapsibleSet("refresh");
		jQuery.Select("ul[data-role='listview']", NodeList).ListView().ListView("refresh");
	}

	/// <summary>
	///　コールバック関数
	/// </summary>
	/// <param name="args"></param>
	private void OnResponse(object args)
	{
		int switchData = (int)((dynamic[])args)[0];
		if (switchData != (int)WAMP_MSG_TYPE.RESULT) {
			return;
		}

		dynamic doc = ((dynamic[])args)[4];
		if (m_DeviceController != null) {
			m_DeviceController.RecvResponse(new WebApiEchonetMessage(doc));
			if (PropertyList == null)
				return;

			m_CurrentNode.GetData();
		}
		SetPropertyBtn.Disable(true);
	}

	private void OnSetResponse(object args)
	{
		dynamic doc = ((dynamic[])args)[4];
		if (m_DeviceController != null) {
			if (PropertyList == null)
				return;
			m_DeviceController.RecvResponse(new WebApiEchonetMessage(doc));
		}
		SetPropertyBtn.Disable(true);
	}

	/// <summary>
	/// #li_device_XXXXXXをクリックしたとき
	/// </summary>
	/// <param name="elem"></param>
	/// <param name="ev"></param>
	private void listitem_Click(Element elem, jQueryEvent ev, DeviceController.DeviceInfo di)
	{
		if (PropertyList == null)
			return;
		m_CurrentNode = di;
	}

	/// <summary>
	/// webapiと接続された時、機器検索のアイコンを切り替える
	/// </summary>
	private void WampClientConnected()
	{
		//接続スイッチがOFFの時、スイッチを切り替える
		bool flag = ConnectionSwt.Prop("checked");
		if (!flag) {
			ConnectionSwt.Prop("checked", true).FlipSwitch("refresh");
		}
		SearchBtn.ToggleClass("ui-icon-alert", "ui-icon-refresh");
		sendHello();
	}

	/// <summary>
	/// webapiと切断されたとき、機器検索のアイコンを切り替える
	/// </summary>
	private void WampClientDisconnected()
	{
		SearchBtn.ToggleClass("ui-icon-refresh", "ui-icon-alert");
	}

	/// <summary>
	/// 接続スイッチを変更
	/// </summary>
	/// <param name="ele"></param>
	public void Connect(Element ele, jQueryEvent eve)
	{
		bool connect = jQuery.Select(ele).Prop("checked");

		//接続スイッチがOn && m_WampClient.isConnecting() == false
		if (connect && !m_WampClient.isConnecting()) {
			Debug.WriteLine("m_WampClient.isConnecting():" + m_WampClient.isConnecting().ToString());
			m_WampClient.connect("ws://" + Window.Location.Host + "/webapi.ashx");
		}
		//接続スイッチがOff && m_WampClient.isConnecting() ==true
		else if (!connect && m_WampClient.isConnecting()) {
			m_WampClient.disconnect();
			//  m_WampClient.sendGoodbye("{}", "wamp.error.system_shutdown",Disconnect);
		}
	}

	/// <summary>
	/// 切断
	/// </summary>
	/// <param name="args"></param>
	public void Disconnect(object args)
	{
		m_WampClient.disconnect();
	}

	int TimerId = 0;

	/// <summary>
	/// 機器検索ボタンを押下
	/// </summary>
	/// <param name="ele"></param>
	public void Search(Element ele, jQueryEvent ev)
	{
		jQuery.Document.Trigger(UIEventNames.SearchStart);
		string data = m_WampClient.sendCall("{}", "com.sonycsl.kadecot.provider.procedure.getDeviceList",
			null, null, OnDevListed);
		if (data == null) {
			jQuery.Document.Trigger(UIEventNames.SearchError);
			return;
		}
		this.TimerId = Window.SetInterval(() => {
			jQuery.Document.Trigger(UIEventNames.SearchError);
		}, 20000);
	}

	public void SearchStart(jQueryEvent ev)
	{
		jQueryMobile.Loading("show", new LoadingOption("機器検索中...", true));
		SearchBtn.Disable(true);
		Debug.WriteLine("機器検索開始");
	}

	/// <summary>
	/// 機器が見つかった時実行
	/// </summary>
	/// <param name="ev"></param>
	public void SearchEnd(jQueryEvent ev)
	{
		if (this.TimerId != 0) {
			Window.ClearInterval(this.TimerId);
			this.TimerId = 0;
		}

		jQueryMobile.Loading("hide");
		SearchBtn.Disable(false);
		NodeList.Find("li .ui-icon-alert").ToggleClass("ui-icon-alert", "ui-icon-carat-r");

		Debug.WriteLine("機器検索終了");
	}

	/// <summary>
	/// 機器が見つからなかったとき実行
	/// </summary>
	/// <param name="ev"></param>
	public void SearchError(jQueryEvent ev)
	{
		if (this.TimerId != 0) {
			Window.ClearInterval(this.TimerId);
			this.TimerId = 0;
		}

		jQueryMobile.Loading("hide");
		jQuery.Select("#bt_search").RemoveAttr("disabled");
		Debug.WriteLine("機器検索エラー");
		Window.Alert("機器が見つかりませんでした");
		NodeList.Find("li .ui-icon-carat-r").ToggleClass("ui-icon-carat-r", "ui-icon-alert");
	}

	public void PropertyWriteStart(jQueryEvent ev)
	{
		jQueryMobile.Loading("show");
	}

	public void PropertyWriteEnd(jQueryEvent ev)
	{
		//PropertyList.Find("li").Find(".ui-field-contain").Children(".ui-input-text").Each((i, ele) => {
		//	if (jQuery.Select(ele).Children(".ui-input-text").Length > 0) {
		//		jQuery.Select(ele).Children("a").Remove();
		//		jQuery.Select(ele).Children(".ui-input-text").Unwrap();
		//	}
		//});
		jQueryMobile.Loading("hide");
	}

	/// <summary>
	/// 設定ボタンを押下
	/// </summary>
	/// <param name="ele"></param>
	public void SetProperty(Element ele, jQueryEvent eve)
	{
		WebApiSet esv = new WebApiSet();
		jQueryObject ctrls = PropertyList;

		esv.deojid = (ENOD_ID)m_CurrentNode.EObjId;
		bool enable = false;

		foreach (var dpi in m_CurrentNode.Properties) {
			if (dpi.InputData == null) {
				continue;
			}
			else if (dpi.IsWaitRes) {
				esv.AddEdt(dpi.PropertyCode, dpi.InputData);
				enable = true;
			}
		}

		if (!enable) {
			return;
		}
		string call = m_WampClient.sendCall(esv.GetOption(), "jp.toppers.ecnl.procedure.set", "[]", esv.GetArguments(),
			   OnSetResponse);
		if (call == null) {
			Window.Alert("機器が接続されていません");
		}
	}

	/// <summary>
	/// 取得ボタンを押下
	/// </summary>
	/// <param name="ele"></param>
	public void GetProperty(jQueryEvent eve)
	{
		string protocol = ((WampApiKadecotDevice)m_CurrentNode.Data).protocol;
		if (protocol == "arduino") {
			return;
		}

		WebApiGet esv = new WebApiGet();
		//現在選択中のノードからEObjIDを取得
		esv.deojid = (ENOD_ID)m_CurrentNode.EObjId;

		//取得フラグ
		bool enable = false;

		foreach (var dpi in m_CurrentNode.Properties) {

			if (dpi == null) {
				continue;
			}
			dpi.IsWaitRes = false;

			bool target = (dpi.Flag & EPC_FLAG.RULE_GET) != 0;

			//読み込み禁止の場合
			if (!target) {
				dpi.ErrorText = "読み込み禁止です";
				continue;
			}
			dpi.IsWaitRes = true;
			esv.AddEpc(dpi.PropertyCode);
			//読み込み可なのでtrue
			enable = true;
		}

		if (!enable)
			return;

		string call = m_WampClient.sendCall(esv.GetOption(), "jp.toppers.ecnl.procedure.get", "[]", esv.GetArguments(),
			   OnResponse);
		if (call == null) {
			m_CurrentNode.GetDeviceBtn().ToggleClass("ui-icon-carat-r", "ui-icon-alert");
		}
		else {
			m_CurrentNode.GetDeviceBtn().ToggleClass("ui-icon-alert", "ui-icon-carat-r");
		}
	}

	/// <summary>
	/// 設定と取得ボタンを押下
	/// </summary>
	/// <param name="ele"></param>
	public void SetGetProperty(Element ele, jQueryEvent ev)
	{
		WebApiSet esv = new WebApiSet();

		esv.deojid = (ENOD_ID)m_CurrentNode.EObjId;
		bool enable = false;

		foreach (var dpi in m_CurrentNode.Properties) {
			if (dpi.InputData == null) {
				continue;
			}
			else if (dpi.IsWaitRes) {
				esv.AddEdt(dpi.PropertyCode, dpi.InputData);
				enable = true;
			}
		}
		if (!enable) {
			return;
		}
		string call = m_WampClient.sendCall(esv.GetOption(), "jp.toppers.ecnl.procedure.setget", "[]", esv.GetArguments(),
			   OnResponse);
		if (call == null) {
			Window.Alert("機器が接続されていません");
		}
	}

	/// <summary>
	/// プロパティの値が変わった時
	/// </summary>
	/// <param name="ele"></param>
	/// <param name="ev"></param>
	public void PropertyChange(Element ele, jQueryEvent ev)
	{
		bool flag = m_CurrentNode.IsWait();
		SetPropertyBtn.Disable(!flag);
		SetGetPropertyBtn.Disable(!flag);
	}

	/// <summary>
	/// Kadecotボタンをクリックした時に実行します
	/// </summary>
	/// <param name="ele"></param>
	/// <param name="ev"></param>
	public void KadecotClick(Element ele, jQueryEvent ev)
	{
		Window.Open("http://app.kadecot.net/index.html?kip=" + Window.Location.Hostname, "Kadecot");
	}

	DeviceController.DevicePropertyInfo dpi;

	/// <summary>
	/// 書き込み
	/// </summary>
	/// <param name="dpi"></param>
	/// <param name="data"></param>
	public void KadecotSet(DeviceController.DevicePropertyInfo dpi, byte[] data)
	{
		string propName;
		if (dpi.PropertyCode == 0x80) {
			propName = "OperationStatus";
		}
		else {
			propName = "0x" + dpi.PropertyCode.ToString(16);
		}

		var di = m_CurrentNode;
		if (di == null)
			return;

		this.dpi = dpi;
		WampApiKadecotSet msg = new WampApiKadecotSet(di.EObjId, propName, data);

		m_WampClient.sendCall(msg.GetOption(), "com.sonycsl.kadecot.echonetlite.procedure.set", "[]", msg.GetArguments(), KadecotSet);
	}

	/// <summary>
	/// 書き込み
	/// </summary>
	/// <param name="args"></param>
	private void KadecotSet(object args)
	{
		var di = m_CurrentNode;
		dynamic doc = ((dynamic[])args)[4];

		var res = new WampApiKadecotRes(doc);

		if (m_DeviceController != null) {
			m_DeviceController.RecvResponse(di, /*ESV_SET_RES*/0x71, this.dpi.PropertyCode, res.propertyValue);
		}
	}

	public void KadecotGet(DeviceController.DevicePropertyInfo dpi)
	{
		string propName;
		if (dpi.PropertyCode == 0x80) {
			propName = "OperationStatus";
		}
		else {
			propName = "0x" + dpi.PropertyCode.ToString(16);
		}

		var di = m_CurrentNode;
		if (di == null)
			return;

		WampApiKadecotGet msg = new WampApiKadecotGet(di.EObjId, propName);
		this.dpi = dpi;
		m_WampClient.sendCall(msg.GetOption(), "com.sonycsl.kadecot.echonetlite.procedure.get", "[]", msg.GetArguments(), KadecotGet);
	}

	public void KadecotGet(object args)
	{
		var di = m_CurrentNode;
		dynamic doc = ((dynamic[])args)[4];

		var res = new WampApiKadecotRes(doc);

		if (m_DeviceController != null) {
			m_DeviceController.RecvResponse(di, /*ESV_GET_RES*/0x72, dpi.PropertyCode, res.propertyValue);
		}
	}

	/// <summary>
	/// 通信エラー発生時処理
	/// </summary>
	/// <param name="request"></param>
	/// <param name="textStatus"></param>
	/// <param name="error"></param>
	private static void AjaxError(jQueryXmlHttpRequest request, string textStatus, dynamic error, string url)
	{
		var sep = new Regex("(\r\n|\r|\n)");
		var lines = new List<string>();

		lines.Add(textStatus);
		if (error is string) {
			lines.Add((string)error);
		}
		else {
			lines.Add(error.message);

			string stack = error.stack;
			lines.AddRange(stack.Split(sep));
		}

		var html = jQuery.FromHtml("<p>");
		foreach (var line in lines) {
			if (sep.Exec(line) != null)
				continue;
			html.Append(Document.CreateTextNode(line).WholeText);
			html.Append(jQuery.FromHtml("<br/>"));
		}
		html.Append(url + "の読み込みに失敗しました");
		jQuery.Select("#popup_dialog_caption").Text("通信エラー");
		jQuery.Select("#popup_dialog_title").Text("通信エラーが発生しました");
		jQuery.Select("#popup_dialog_text").Html(html);
		jQuery.Select("#popup_dialog").Popup("open", new { transition = "slidedown" });
	}

	/// <summary>
	/// 設定・取得ボタンの有効・無効設定
	/// </summary>
	private void CurrentNodeChange(Element elem, jQueryEvent ev)
	{
		ButtonDisabled(m_CurrentNode);

		if (m_CurrentNode == null) {
			return;
		}
		PropertyCreate(m_CurrentNode);
		GetProperty(ev);
		return;
	}

	/// <summary>
	/// 設定・取得ボタンを設定します
	/// </summary>
	/// <param name="di"></param>
	private void ButtonDisabled(DeviceController.DeviceInfo di)
	{
		if (di == null) {
			SetGetPropertyBtn.Disable(true);
			GetPropertyBtn.Disable(true);
			SetPropertyBtn.Disable(true);
			return;
		}

		var ctrls = PropertyList;

		SetGetPropertyBtn.Disable(ctrls == null);
		GetPropertyBtn.Disable(ctrls == null);
		SetPropertyBtn.Disable(ctrls == null);
	}

	/// <summary>
	/// 入力画面を作成
	/// </summary>
	/// <param name="di"></param>
	private void PropertyCreate(DeviceController.DeviceInfo di)
	{
		if (di == null) {
			return;
		}
		jQuery.Document.Trigger(UIEventNames.PropertyWriteStart);
		di.SetUIDevice(this);

		PropertyList.Html("");
		PropertyList.Append(di.GetPropertys());
		PropertyList.ListView().ListView("refresh");
		PropertyList.ListView().Trigger("create");

		jQuery.Document.Trigger(UIEventNames.PropertyWriteEnd);
	}

	/// <summary>
	/// 初期値設定ボタン
	/// </summary>
	/// <param name="elem"></param>
	/// <param name="e"></param>
	private void SetInitParams(Element elem, jQueryEvent e)
	{
		var macaddrfrm = new Regex("([0-9A-Fa-f]{2}):([0-9A-Fa-f]{2}):([0-9A-Fa-f]{2}):([0-9A-Fa-f]{2}):([0-9A-Fa-f]{2}):([0-9A-Fa-f]{2})");

		var m = macaddrfrm.Exec(MacAddrTxt.GetValue());
		if (m == null)
			return;

		var ma1 = Byte.Parse(m[1], 16);
		var ma2 = Byte.Parse(m[2], 16);
		var ma3 = Byte.Parse(m[3], 16);
		var ma4 = Byte.Parse(m[4], 16);
		var ma5 = Byte.Parse(m[5], 16);
		var ma6 = Byte.Parse(m[6], 16);

		m_WampClient.sendCall("{}", "jp.toppers.ecnl.procedure.setInitParams", "[]",
			"{\"macAddr\":[" + ma1 + "," + ma2 + "," + ma3 + "," + ma4 + "," + ma5 + "," + ma6 + "]}", SetInitParamsRes);
	}

	public void SetInitParamsRes(object args)
	{
		int cmd = (int)((dynamic[])args)[0];
		if (cmd != (int)WAMP_MSG_TYPE.RESULT) {
			jQuery.Select("#popup_dialog_caption").Text("エラー");
			jQuery.Select("#popup_dialog_title").Text("エラーが発生しました");
			jQuery.Select("#popup_dialog_text").Text("初期データの書き込みに失敗しました。");
			jQuery.Select("#popup_dialog").Popup("open", new { transition = "slidedown" });
			return;
		}

		jQuery.Select("#popup_dialog_caption").Text("初期データ");
		jQuery.Select("#popup_dialog_title").Text("初期データを書き込みました");
		jQuery.Select("#popup_dialog_text").Text("初期データの書き込みに成功しました。");
		jQuery.Select("#popup_dialog").Popup("open", new { transition = "slidedown" });
	}

	/// <summary>
	/// 初期値設定ボタン
	/// </summary>
	/// <param name="elem"></param>
	/// <param name="e"></param>
	private void GetInitParams(Element elem, jQueryEvent e)
	{
		m_WampClient.sendCall("{}", "jp.toppers.ecnl.procedure.getInitParams", "[]", "{}", GetInitParamsRes);
	}

	public void GetInitParamsRes(object args)
	{
		int cmd = (int)((dynamic[])args)[0];
		if (cmd != (int)WAMP_MSG_TYPE.RESULT) {
			jQuery.Select("#popup_dialog_caption").Text("エラー");
			jQuery.Select("#popup_dialog_title").Text("エラーが発生しました");
			jQuery.Select("#popup_dialog_text").Text("初期データの読み込みに失敗しました。");
			jQuery.Select("#popup_dialog").Popup("open", new { transition = "slidedown" });
			return;
		}

		var macaddr = (byte[])(((dynamic[])args)[4].macAddr);
		MacAddrTxt.Value(String.Format("{0:X02}:{1:X02}:{2:X02}:{3:X02}:{4:X02}:{5:X02}",
			macaddr[0], macaddr[1], macaddr[2], macaddr[3], macaddr[4], macaddr[5]));
	}

	/// <summary>設定ボタン</summary>
	public jQueryObject SetPropertyBtn { get { return jQuery.Select("#bt_set_property", page); } }

	/// <summary>取得ボタン</summary>
	public jQueryObject GetPropertyBtn { get { return jQuery.Select("#bt_get_property", page); } }

	/// <summary>設定と取得ボタン</summary>
	public jQueryObject SetGetPropertyBtn { get { return jQuery.Select("#bt_set_get_property", page); } }

	/// <summary>Kadecotボタン</summary>
	public jQueryObject KadecotBtn { get { return jQuery.Select("#bt_kadecot", page); } }

	/// <summary>接続スイッチ</summary>
	public jQueryObject ConnectionSwt { get { return jQuery.Select("#fs_connection", page); } }

	/// <summary>プロパティリスト</summary>
	public jQueryObject PropertyList { get { return jQuery.Select("#lv_property_list", page); } }

	/// <summary>機器検索ボタン</summary>
	public jQueryObject SearchBtn { get { return jQuery.Select("#bt_search", page); } }

	/// <summary>ノードリスト</summary>
	public jQueryObject NodeList { get { return jQuery.Select("#node_list", page); } }

	/// <summary>groupBox</summary>
	public jQueryObject PropertyGroupBox { get { return jQuery.Select("#lv_property_list", page).Find("li"); } }

	/// <summary>初期値設定ボタン</summary>
	public jQueryObject SetInitParamsBtn { get { return jQuery.Select("#bt_set_init_params", page); } }

	/// <summary>初期値取得ボタン</summary>
	public jQueryObject GetInitParamsBtn { get { return jQuery.Select("#bt_get_init_params", page); } }

	/// <summary>MACアドレス入力</summary>
	public jQueryObject MacAddrTxt { get { return jQuery.Select("#txt_mac_addr", page); } }

	public void pinMode_Change(UIFieldSet fieldset, int pin)
	{
		string value = fieldset.CheckdValue();
		JsDictionary dic = new JsDictionary("pin", pin, "mode", value);
		m_WampClient.sendCall("{\"deviceId\":" + 1 + "}", "com.sonycsl.kadecot.arduino.pinMode", "[]", Json.Stringify(dic), pinMode);
	}

	public void digital_Change(UIButton button, UIFlipSwitch flipswitch, int pin)
	{
		WampApiKadecotSet msg = new WampApiKadecotSet(1, "2", new byte[2]);
		JsDictionary dic = new JsDictionary("pin", pin, "value", flipswitch.Value);
		m_WampClient.sendCall("{\"deviceId\":" + 1 + "}", "com.sonycsl.kadecot.arduino.digitalWrite", "[]", Json.Stringify(dic), digitalWrite);
	}

	public void analogWrite_Change(UIRangeInput range, int pin)
	{
		WampApiKadecotSet msg = new WampApiKadecotSet(1, "2", new byte[2]);
		JsDictionary dic = new JsDictionary("pin", pin, "value", range.Value);
		m_WampClient.sendCall("{\"deviceId\":" + 1 + "}", "com.sonycsl.kadecot.arduino.analogWrite", "[]", Json.Stringify(dic), pinMode);
	}

	public void analogRead(UIRangeInput range, int pin)
	{
		JsDictionary dic = new JsDictionary("pin", pin);
		m_WampClient.sendCall("{\"deviceId\":" + 1 + "}", "com.sonycsl.kadecot.arduino.analogRead", "[]", Json.Stringify(dic), (args) => { digitalRead(args, range); });
	}

	public void pinMode(object args)
	{
		int switchData = (int)((dynamic[])args)[0];
		if (switchData != (int)WAMP_MSG_TYPE.RESULT) {
			return;
		}
		dynamic doc = ((dynamic[])args)[4];
		if (m_DeviceController != null) {
			if (PropertyList == null)
				return;
		}
	}

	public void digitalWrite(object args)
	{
		int switchData = (int)((dynamic[])args)[0];
		if (switchData != (int)WAMP_MSG_TYPE.RESULT) {
			return;
		}
		dynamic doc = ((dynamic[])args)[4];
		if (m_DeviceController != null) {
			if (PropertyList == null)
				return;
		}
	}

	public void DigitalRead(UIButton button, UIFlipSwitch flip, int i)
	{
		JsDictionary dic = new JsDictionary("pin", i);
		string str = m_WampClient.sendCall("{\"deviceId\":" + 1 + "}", "com.sonycsl.kadecot.arduino.digitalRead", "[]", Json.Stringify(dic), (arg) => { digitalRead(arg, flip); });
		if (str == null) {
			button.ToggleClass("ui-icon-refresh", "ui-icon-alert");
		}
		else {
			button.ToggleClass("ui-icon-alert", "ui-icon-refresh");
		}
	}

	public void digitalRead(object args, UIObject uiobject)
	{
		int switchData = (int)((dynamic[])args)[0];
		if (switchData != (int)WAMP_MSG_TYPE.RESULT) {
			return;
		}
		dynamic doc = ((dynamic[])args)[4];
		if (m_DeviceController != null) {

			WebApiGrsaguraMessage msg = new WebApiGrsaguraMessage(doc);
			if (uiobject.GetType() == typeof(UIFlipSwitch)) {
				UIFlipSwitch flip = (UIFlipSwitch)uiobject;
				flip.Value = msg.value;
			}
			else if (uiobject.GetType() == typeof(UIRangeInput)) {
				UIRangeInput range = (UIRangeInput)uiobject;
				range.Value = int.Parse(msg.value);
				range.Object.Slider("refresh");
			}
		}
	}
}

namespace ctrlui
{
	public class Information
	{
		private jQueryObject info;

		public Information()
		{
		}

		public void Update(Element ele, string id)
		{
			var page = CtrlUI.GetPage(ele);

			this.info = jQuery.Select("#" + id, page);
			if (this.info == null)
				return;
			jQuery.Ajax("./" + id + ".json").Success(this.Success).Error(this.Error);
		}

		private void Success(object data, string textStatus, jQueryXmlHttpRequest request)
		{
			var items = (dynamic[])((data.GetType() == typeof(string)) ? jQuery.ParseJson((string)data) : data);

			var listview = new UIListview("data-inset", true, "data-theme", "b").Object;
			listview.Append(new UIListDivider("ソフトウェア情報").Object);

			foreach (var item in items) {
				string title = item.title;
				string version = item.version;
				string link = item.link;
				string note = item.note;

				UIAnchor a;
				if (String.IsNullOrEmpty(link)) {
					a = new UIAnchor(new UIH(2, title));
					a.Href = "#";
				}
				else {
					a = new UIAnchor(new UIH(2, title));
					a.Href = link;
					a.Target = "_blank";
				}
				a.Append(new UIP("Version " + version));
				if (!String.IsNullOrEmpty(note))
					a.Append(new UIP(note));
				listview.Append(new UIListviewItem(a).Object);
			}
			UIAnchor anchor = new UIAnchor();
			anchor.Target = "_blank";
			anchor.Href = "http://www.core-s.co.jp";
			anchor.Append(new UIImg("src", "imgs/core-s.svg").Object).Append("コアーズ株式会社");

			var listitem = new UIListDivider(anchor).Object;

			listview.Append(listitem);
			this.info.Html("");
			this.info.Append(listview);

			jQuery.Select("ul", this.info).ListView().ListView("refresh");
			this.info.Popup("reposition", new { positionTo = "origin" });
		}

		private void Error(jQueryXmlHttpRequest request, string textStatus, Exception error)
		{
			jQueryObject listview = new UIListview("data-inset", "true", "data-theme", "b").Object;
			listview.Append(new UIListDivider("ソフトウェア情報").Object);
			listview.Append(new UIListviewItem(new UIP("通信エラー")).Object);

			this.info.Append(listview);
			jQuery.Select("ul", this.info).ListView().ListView("refresh");
			this.info.Popup("reposition", new { positionTo = "origin" });
		}
	}
}
