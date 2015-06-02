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

namespace control
{
	public static class UI
	{
		public static DeviceController.DeviceInfo GetDeviceInfo(this JsonClassInfo dc, WebApiObjectInfo dev, WampApiKadecotDevice device)
		{
			DeviceController.DeviceInfo di = new DeviceController.DeviceInfo(dc, dev);
			di.Data = device;
			if (dc != null) {
				foreach (WebApiPropertyInfo prop in dev.properties) {
					JsonPropertyInfo epi = dc.properties.FirstOrDefault(p => p.propertyCode == prop.epc);
					if (epi == null)
						continue;
					di.Properties.Add(new DeviceController.DevicePropertyInfo(epi, prop));
				}
			}
			return di;
		}

		public static List<WampApiKadecotDevice> GetAllDevices(this object args)
		{
			dynamic[] devs = (dynamic[])(((dynamic[])args)[4]).deviceList;
			List<WampApiKadecotDevice> alldevs = new List<WampApiKadecotDevice>();
			var nodes = new Dictionary<string, List<WampApiKadecotDevice>>();

			foreach (dynamic dev in devs) {
				WampApiKadecotDevice device = new WampApiKadecotDevice(dev);
				List<WampApiKadecotDevice> devices;

				if (nodes.TryGetValue(device.ip_addr, out devices)) {
					devices.Add(device);
				}
				else {
					devices = new List<WampApiKadecotDevice>();
					devices.Add(device);
					nodes[device.ip_addr] = devices;
				}

				alldevs.Add(device);
			}

			return alldevs;
		}

		/// <summary>
		/// jsonファイルをリストに変換します
		/// </summary>
		/// <param name="data"></param>
		/// <returns></returns>
		public static List<JsonClassGroupInfo> ToJSClassGroupInfoList(this object data)
		{
			dynamic[] _classGroups = (dynamic[])((data.GetType() == typeof(string)) ? jQuery.ParseJson((string)data) : data);
			List<JsonClassGroupInfo> classGroups = new List<JsonClassGroupInfo>();
			foreach (var _item in _classGroups) {
				var item = new JsonClassGroupInfo(_item);
				classGroups.Add(item);
			}
			return classGroups;
		}

		/// <summary>
		/// jsonファイルをリストに変換します
		/// </summary>
		/// <param name="data"></param>
		/// <param name="classGroup">クラス</param>
		/// <returns></returns>
		public static List<JsonClassInfo> ToJSClassInfoList(this object data, JsonClassGroupInfo classGroup)
		{
			dynamic[] _classes = (dynamic[])((data.GetType() == typeof(string)) ? jQuery.ParseJson((string)data) : data);
			List<JsonClassInfo> classes = new List<JsonClassInfo>();
			JsonClassInfo eclass;
			foreach (var _item in _classes) {
				eclass = new JsonClassInfo(_item);
				eclass.classGroup = classGroup;
				classes.Add(eclass);
			}
			return classes;
		}

		/// <summary>
		/// jsonファイルをプロパティリストに変換します
		/// </summary>
		/// <param name="data"></param>
		/// <returns></returns>
		public static List<JsonPropertyInfo> ToJSPropertyInfoList(this object data)
		{
			var _properties = (dynamic[])((data.GetType() == typeof(string)) ? jQuery.ParseJson((string)data) : data);
			var properties = new List<JsonPropertyInfo>();
			foreach (var _item in _properties) {
				var item = new JsonPropertyInfo(_item);
				properties.Add(item);
			}
			return properties;
		}

		/// <summary>
		/// disabledをタグに追加します
		/// </summary>
		/// <param name="obj"></param>
		/// <param name="flag">true: disabledをタグに追加　false: disabledをタグから削除</param>
		/// <returns></returns>
		public static jQueryObject Disable(this jQueryObject obj, bool flag)
		{
			if (flag) {
				obj.Attribute("disabled", "disabled");
			}
			else {
				obj.RemoveAttr("disabled");
			}

			return obj;
		}

		/// <summary>
		/// クラスを入れ替えます
		/// </summary>
		/// <param name="obj"></param>
		/// <param name="deleteClass">削除するクラス</param>
		/// <param name="addClass">追加するクラス</param>
		/// <returns></returns>
		public static jQueryObject ToggleClass(this jQueryObject obj, string deleteClass, string addClass)
		{
			obj.ToggleClass(deleteClass, false);
			obj.ToggleClass(addClass, true);
			return obj;
		}

		public static jQueryObject GetDeviceBtn(this DeviceController.DeviceInfo di)
		{
			return di.GetDeviceLi().Children("a");
		}

		public static jQueryObject GetDeviceLi(this DeviceController.DeviceInfo di)
		{
			return jQuery.Select("#li_device_" + di.GetObjectID());
		}

		/// <summary>
		/// 書き込み可であるかどうかしらべます
		/// </summary>
		/// <returns>true:書き込み可,false:書き込み不可</returns>
		public static bool IsRULE_SET(this DeviceController.DevicePropertyInfo dpi)
		{
			return (dpi.PropertyInfo == null) ? false : dpi.PropertyInfo.access.Contains("RULE_SET");
		}
	}

	public class UIObject
	{
		public jQueryObject Object;
		public Element Element { get { return Object.GetElement(0); } }

		public UIObject()
		{
		}

		public UIObject(UIObject uiobject)
		{
			this.Object = uiobject.Object;
		}

		public UIObject(jQueryObject @object)
		{
			this.Object = @object;
		}

		public UIObject(string html)
		{
			this.Object = jQuery.FromHtml(html);
		}

		public UIObject Change()
		{
			this.Object.Change();
			return this;
		}

		public void Change(jQueryEventHandlerWithContext eventHandler)
		{
			Object.Change(eventHandler);
		}

		public void Keypress(jQueryEventHandlerWithContext eventHandler)
		{
			Object.Keypress(eventHandler);
		}

		public jQueryObject Toggle(bool showOrHide)
		{
			return this.Object.Toggle(showOrHide);
		}

		public void Attribute(string attributeName, string value)
		{
			this.Object.Attribute(attributeName, value);
		}

		public void RemoveAttr(string attributeName)
		{
			this.Object.RemoveAttr(attributeName);
		}

		public string GetAttribute(string attributeName)
		{
			return Object.GetAttribute(attributeName);
		}

		public jQueryObject Find(string selector)
		{
			return this.Object.Find(selector);
		}

		public UIObject RemoveClass(string className)
		{
			this.Object.RemoveClass(className);
			return this;
		}

		public UIObject ToggleClass(string className, bool add)
		{
			this.Object.ToggleClass(className, add);
			return this;
		}

		public UIObject ToggleClass(string deleteClass, string addClass)
		{
			this.Object.ToggleClass(deleteClass, addClass);
			return this;
		}

		public UIObject One(string eventName, jQueryEventHandlerWithContext eventHandler)
		{
			this.Object.One(eventName, eventHandler);
			return this;
		}

		public UIObject Attribute(JsDictionary nameValueMap)
		{
			this.Object.Attribute(nameValueMap);
			return this;
		}

		public UIObject Click(jQueryEventHandlerWithContext eventHandler)
		{
			this.Object.Click(eventHandler);
			return this;
		}

		public UIObject Click(jQueryEventHandler eventHandler)
		{
			this.Object.Click(eventHandler);
			return this;
		}

		public string Text { get { return Object.GetText(); } set { Object.Text(value); } }
		public string Id { get { return Object.GetAttribute("id"); } set { Object.Attribute("id", value); } }
		public string For { get { return Object.GetAttribute("for"); } set { Object.Attribute("for", value); } }
		public string Name { get { return Object.GetAttribute("name"); } set { Object.Attribute("name", value); } }
		public string DataRole { get { return Object.GetAttribute("data-role"); } set { Object.Attribute("data-role", value); } }
		public string DataType { get { return Object.GetAttribute("data-type"); } set { Object.Attribute("data-type", value); } }
		public string Title { get { return Object.GetAttribute("title"); } set { Object.Attribute("title", value); } }
		public string Value
		{
			get
			{
				if (Object.Is("fieldset"))
					return Object.Find("input[type='radio']:checked").GetValue();
				return Object.GetValue();
			}
			set
			{
				if (Object.Is("fieldset")) {
					Object.Find("input[type=\"radio\"]").Each((j, elem) => {
						if (jQuery.Select(elem).GetValue() == value) {
							jQuery.Select(elem).Prop("checked", true);
						}
						else {
							jQuery.Select(elem).Prop("checked", false);
						}
					});
					Object.Find("input[type=\"radio\"]").CheckboxRadio().CheckboxRadio("refresh");
				}
				else {
					Object.Value(value);
					if (DataRole == "flipswitch")
						Object.Value(value).FlipSwitch("refresh");
				}
			}
		}

		public int Maxlength { get { return int.Parse(GetAttribute("maxlength")); } set { Attribute("maxlength", value.ToString()); } }
		public int Min { get { return int.Parse(Object.GetAttribute("min")); } set { Object.Attribute("min", value.ToString()); } }
		public int Max { get { return int.Parse(Object.GetAttribute("max")); } set { Object.Attribute("max", value.ToString()); } }
		public int TabIndex { get { return int.Parse(Object.GetAttribute("tabindex")); } set { Object.Attribute("tabindex", value.ToString()); } }
		public string Type { get { return Object.GetAttribute("type"); } set { Object.Attribute("type", value); } }

		public void Append(UIObject uiobject)
		{
			this.Object.Append(uiobject.Object);
		}

		public jQueryObject Append(jQueryObject @object)
		{
			return this.Object.Append(@object);
		}

		public UIObject Disable(bool flag)
		{
			if (flag) {
				this.Attribute("disabled", "disabled");
			}
			else {
				this.RemoveAttr("disabled");
			}
			return this;
		}

		public jQueryObject Children()
		{
			return this.Object.Children();
		}

		public UIObject AddClass(string className)
		{
			this.Object.AddClass(className);
			return this;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="flag">false:イベント不可</param>
		/// <returns></returns>
		public UIObject PointerEvents(bool flag)
		{
			if (!flag) {
				this.Object.CSS("pointer-events", "none");
			}
			return this;
		}

		public UIObject Hide()
		{
			this.Object.Hide();
			return this;
		}

		public UIObject Show()
		{
			this.Object.Show();
			return this;
		}

		private EcnlClass ecnlclass;
		public EcnlClass Class
		{
			get { return ecnlclass; }
			set
			{
				ecnlclass = value;
				this.AddClass(EcnlClassString(value));
			}
		}

		public enum EcnlClass
		{
			ecnl_pink,
			ecnl_violet,
			ecnl_light_green,
			ecnl_light_blue,
		}

		private string EcnlClassString(EcnlClass @class)
		{
			if (@class == EcnlClass.ecnl_pink) {
				return "ecnl_pink";
			}
			else if (@class == EcnlClass.ecnl_violet) {
				return "ecnl_violet";
			}
			else if (@class == EcnlClass.ecnl_light_green) {
				return "ecnl_light_green";
			}
			else if (@class == EcnlClass.ecnl_light_blue) {
				return "ecnl_light_blue";
			}
			else {
				return "";
			}
		}
	}

	public class UIGrpBox : UIListviewItem
	{
		UILabel Label;

		public UIGrpBox()
			: base()
		{
			LiAside = new UIP("", "ui-li-aside");
			base.Append(LiAside);
			base.Change(GrpBoxChange);
		}

		public UIGrpBox(DeviceController.DevicePropertyInfo dpi)
			: this()
		{
			this.dpi = dpi;
			Label = new UILabel(dpi);
			this.dpi.PrepareInputInfo();

			if (dpi.PropertyCode == 0x00) {

			}
			else if (dpi.PropertyCode == 0x97) {
				SetTimebox();
			}
			else if (dpi.PropertyCode == 0x98) {
				SetCalBox();
			}
			else if (this.dpi.InputTypes.Count == 0) {
				SetTextInput();
			}
			else if (dpi.PropertyCode == 0x80 && dpi.InputTypes.Count == 1 && dpi.InputTypes[0].Mode == PropertyInputMode.Select) {
				SetFlipSwitch();
			}
			else {
				foreach (PropertyInputInfo pii in dpi.InputTypes) {

					switch (pii.Mode) {
					case PropertyInputMode.Select: {
							SetFieldSet(pii);
						}
						break;
					case PropertyInputMode.Range: {
							SetRange(pii);
						}
						break;
					default: {
							SetDefault();
						}
						break;
					}
				}
			}
		}

		public void SetFlipSwitch()
		{
			PropertySelectInput psi = (PropertySelectInput)dpi.InputTypes[0];

			// OFF
			var off = psi.Option.First((kvp) => { return kvp.Key == 0x31; });

			// ON
			var on = psi.Option.First((kvp) => { return kvp.Key == 0x30; });

			UIOption OFF = new UIOption(off.Value, off.Key.ToString("X2"));
			UIOption ON = new UIOption(on.Value, on.Key.ToString("X2"));

			UIFlipSwitch Flipswitch = new UIFlipSwitch(OFF, ON);
			Flipswitch.Change(this.flip_Change);

			Flipswitch.Disable(!dpi.IsRULE_SET());

			FieldContain = new UIFieldContain(Label, Flipswitch);

			//FieldContain.Object.Trigger("create");
			Append(FieldContain);
		}

		public void SetDefault()
		{
			UITextInput Textbox = new UITextInput();
			Textbox.Change(text_Change);
			var value = ""/*.ToString("X")*/;
			Textbox.Value = value;
			FieldContain = new UIFieldContain(Label, Textbox);
			Append(FieldContain);
		}

		public void SetRange(PropertyInputInfo pii)
		{
			UIRangeInput range = new UIRangeInput();
			range.Change(range_Change);
			FieldContain = new UIFieldContain(Label, range);
			var value = ((PropertyRangeInput)pii).Minimum;

			range.Value = value;
			range.Min = ((PropertyRangeInput)pii).Minimum;
			range.Max = ((PropertyRangeInput)pii).Maximum;

			Append(FieldContain.Object).Trigger("create");
		}

		private void range_Change(Element elem, jQueryEvent ev)
		{
			if (!CheckValiable()) { return; };
			if (!CheckHex()) { return; };
			byte[] data = ByteValue;
			dpi.InputData = data;
			KadecotSet(dpi, data);
		}

		private void SetCalBox()
		{
			UICalBox Textbox = new UICalBox();
			Textbox.Change(calbox_Change);

			JsonPropertyInfo epi = dpi.PropertyInfo;

			FieldContain = new UIFieldContain(Label, Textbox);

			int len = dpi.GetMaxLength();

			string valueDescription = "";
			foreach (JsonFieldInfo jfi in epi.fields) {
				valueDescription += "【" + jfi.description + ":" + jfi.valueDescription + "】";
			}
			Textbox.Placeholder = dpi.GetText();
			Textbox.Value = "";
			//Textbox.Disable(!dpi.IsRULE_SET());
			Textbox.ReadOnly = !dpi.IsRULE_SET();
			Textbox.Title = valueDescription;
			One("dateboxcreate", calbox_Create);
			Append(FieldContain.Object);
		}

		private void calbox_Create(Element ele, jQueryEvent ev)
		{
			var textinput = jQuery.Select(ev.Target).Parent(".ui-input-datebox");
			textinput.Append(BackButton.Object);
			BackButton.Click(BackButtonClick);
		}

		private void calbox_Change(Element elem, jQueryEvent ev)
		{
			UICalBox Calbox = (UICalBox)FieldContain.Input;
			byte[] data = new byte[4];
			RemoveClass();

			if (!((Calbox.dY >= 1 && Calbox.dY < 9999) && ((Calbox.dM >= 1 && Calbox.dM <= 12) && (Calbox.dD >= 1 && Calbox.dD <= 31)))) {
				ToggleClass(true ? "ecnl_pink" : "ecnl_violet", true);
			}
			else {
				data[0] = Byte.Parse(Calbox.xY.Substring(0, 2), 16);
				data[1] = Byte.Parse(Calbox.xY.Substring(2, 2), 16);
				data[2] = Byte.Parse(Calbox.xM, 16);
				data[3] = Byte.Parse(Calbox.xD, 16);

				dpi.InputData = data;
				dpi.IsWaitRes = true;

				this.Class = EcnlClass.ecnl_light_green;
			}
		}

		private void flip_Change(Element elem, jQueryEvent ev)
		{
			byte[] data = ByteValue;
			dpi.InputData = data;
			KadecotSet(dpi, data);
		}

		private void SetTextInput()
		{
			UITextInput Textbox = new UITextInput();
			Textbox.Change(text_Change);
			Textbox.Keypress(text_KeyPress);
			Textbox.Keyup(text_Keyup);
			Textbox.On("textinputcreate", this.text_Create);
			JsonPropertyInfo epi = dpi.PropertyInfo;

			string valueDescription = "";
			foreach (JsonFieldInfo jfi in epi.fields) {
				valueDescription += "【" + jfi.description + ":" + jfi.valueDescription + "】";
			}
			Textbox.Value = "";
			//Textbox.Disable(!dpi.IsRULE_SET());
			Textbox.ReadOnly = !dpi.IsRULE_SET();
			Textbox.Maxlength = dpi.GetMaxLength();
			Textbox.Title = valueDescription;
			FieldContain = new UIFieldContain(Label, Textbox);
			base.Append(FieldContain.Object);
		}

		private void SetTimebox()
		{
			UITimeBox Textbox = new UITimeBox();
			Textbox.Change(timebox_Change);
			JsonPropertyInfo epi = dpi.PropertyInfo;
			FieldContain = new UIFieldContain(Label, Textbox);
			int len = dpi.GetMaxLength();
			string valueDescription = "";

			foreach (JsonFieldInfo jfi in epi.fields) {
				valueDescription += "【" + jfi.description + ":" + jfi.valueDescription + "】";
			}

			Textbox.Placeholder = dpi.GetText();
			Textbox.Value = "";
			Textbox.Title = valueDescription;
			//Textbox.Disable(!dpi.IsRULE_SET());
			Textbox.ReadOnly = !dpi.IsRULE_SET();
			this.Append(FieldContain);
			this.One("dateboxcreate", timebox_Create);
		}

		private void SetFieldSet(PropertyInputInfo pii)
		{
			UIFieldSet FieldSet = new UIFieldSet();
			UILabel Label = new UILabel();
			Label.Text = dpi.GetText();
			FieldSet.Horizontal = true;
			FieldSet.Change(select_Change);
			FieldContain = new UIFieldContain(Label, FieldSet);

			foreach (KeyValuePair<long, string> opt in ((PropertySelectInput)pii).Option) {
				UIRadioInput Radio = new UIRadioInput(opt.Key.ToString("X"));
				FieldSet.Append(new UIFieldSet.RadioInput(new UILabel(opt.Value), Radio));
			}

			Append(FieldContain.Object);
		}

		private void select_Change(Element elem, jQueryEvent ev)
		{
			byte[] data = ByteValue;
			dpi.InputData = data;
			KadecotSet(dpi, data);
		}

		/// <summary>
		/// 時刻入力フォームの入力値が変更された場合
		/// </summary>
		/// <param name="ele"></param>
		/// <param name="ev"></param>
		private void timebox_Change(Element ele, jQueryEvent ev)
		{
			RemoveClass();
			if (Changed) {
				timebox_Change();
			}
		}

		private void timebox_Change()
		{
			string value = FieldContain.Input.Value;
			string[] val = value.Split(":");

			Byte[] data = new Byte[2];
			data[0] = byte.Parse(val[0], 10);
			data[1] = byte.Parse(val[1], 10);

			string hex = data[0].ToString("X2") + data[1].ToString("X2");

			RemoveClass();

			dpi.InputData = data;
			dpi.IsWaitRes = true;
			ToggleClass(true ? "ecnl_light_green" : "ecnl_light_blue", true);
		}

		private void timebox_Create(Element ele, jQueryEvent ev)
		{
			var textinput = jQuery.Select(ev.Target).Parent(".ui-input-datebox");
			textinput.Append(BackButton.Object);
			BackButton.Click(BackButtonClick);
		}

		private void text_Keyup(Element elem, jQueryEvent ev)
		{
			jQuery.Select(elem).Change();
		}

		private void text_Create(Element elem, jQueryEvent ev)
		{
			var textinput = jQuery.Select(ev.Target).Parent(".ui-input-text");
			textinput.Append(BackButton.Object);
			BackButton.Click(BackButtonClick);
		}

		private void text_KeyPress(Element elem, jQueryEvent ev)
		{
			bool number = (ev.Which >= (int)KeyPress._0 && ev.Which <= (int)KeyPress._9);
			bool lower = (ev.Which >= (int)KeyPress.a && ev.Which <= (int)KeyPress.f);
			bool upper = (ev.Which >= (int)KeyPress.A && ev.Which <= (int)KeyPress.F);
			bool BackSpace = (ev.Which == (int)KeyPress.BackSpace);
			bool Delete = (ev.Which == (int)KeyPress.Delete);

			bool flag = (!number && !lower) && (!BackSpace && !Delete) && !upper;
			if (flag) ev.PreventDefault();
		}

		private void BackButtonClick(Element elem, jQueryEvent ev)
		{
			FieldContain.Input.Value = ToText(GetValue);
			FieldContain.Input.Object.Change();
		}

		private void text_Change(Element elem, jQueryEvent ev)
		{
			if (Changed) {
				RemoveClass();
				if (!CheckValiable()) { return; };
				if (!CheckHex()) { return; };
				SetDpi();
			}
		}

		private Type InputType { get { return FieldContain.Input.GetType(); } }
		private byte[] ToByteValue()
		{
			string Value = FieldContain.Input.Value;
			byte[] data = new byte[Value.Length / 2];
			for (int i = 0, j = 0; j < Value.Length; j += 2) {
				data[i++] = Byte.Parse(Value.Substring(j, 2), 16);
			}
			return data;
		}

		private void SetDpi()
		{
			dpi.InputData = ByteValue;
			dpi.IsWaitRes = true;
			ErrorText = "";
			ToggleClass(user ? "ecnl_light_green" : "ecnl_light_blue", true);
		}

		bool user = true;
		private byte[] ByteValue { get { return ToByteValue(); } }
		private DeviceController.DevicePropertyInfo dpi;
		private UIP LiAside;
		public string ErrorText { get { return LiAside.Text; } set { LiAside.Text = value; } }
		public byte[] GetValue;
		public UIFieldContain FieldContain;

		public void RemoveClass()
		{
			ErrorText = "";
			BackButton.Hide();
			RemoveClass("ecnl_pink");
			RemoveClass("ecnl_violet");
			RemoveClass("ecnl_light_green");
			RemoveClass("ecnl_light_blue");
		}

		public void SetName(string nameid)
		{
			string name = nameid;
			if (InputType == typeof(UICalBox)) {
				name = "cal_" + nameid;
			}
			else if (InputType == typeof(UITimeBox)) {
				name = "tmb_" + nameid;
			}
			else if (InputType == typeof(UITextInput)) {
				name = "txt_" + nameid;
			}
			else if (InputType == typeof(UIRadioInput)) {
				name = "rng_" + nameid;
			}
			else if (InputType == typeof(UIFlipSwitch)) {
				name = "flp_" + nameid;
				UIFlipSwitch Flipswitch = (UIFlipSwitch)FieldContain.Input;

				string val0 = Flipswitch.OFF.Value;
				string val1 = Flipswitch.ON.Value;
				Flipswitch.OFF.Id = name + val0;
				Flipswitch.ON.Id = name + val1;
			}
			else if (InputType == typeof(UIFieldSet)) {
				name = "sel_" + nameid;
				UIFieldSet FieldSet = (UIFieldSet)FieldContain.Input;
				FieldSet.Id = name;

				for (int i = 0; i < FieldSet.RadioList.Count; i++) {
					var id = name + FieldSet.RadioList[i].Radio.Value;
					FieldSet.RadioList[i].Radio.Name = name;
					FieldSet.RadioList[i].Radio.Id = id;
					FieldSet.RadioList[i].Label.For = id;
				}
			}

			FieldContain.Label.For = name;
			FieldContain.Input.Id = name;
			FieldContain.Input.Name = name;
		}

		public bool CheckHex()
		{
			string Value = FieldContain.Input.Value;
			if (!new Regex("^([0-9A-Fa-f][0-9A-Fa-f])+$").Test(Value)) {
				ErrorText = "16進数で入力してください";
				ToggleClass(true ? "ecnl_pink" : "ecnl_violet", true);
				return false;
			}
			return true;
		}

		public bool CheckValiable()
		{
			JsonPropertyInfo epi = dpi.PropertyInfo;
			//可変長データの場合
			bool valiable = dpi.IsValiable();
			int len = dpi.GetMaxLength();
			string Value = FieldContain.Input.Value;
			bool length = (Value.Length != len) && (!valiable || (Value.Length > len));

			if ((Value.Length != len) && (!valiable || (Value.Length > len))) {
				ErrorText = len.ToString() + "文字で入力してください";
				ToggleClass(user ? "ecnl_pink" : "ecnl_violet", true);

				return false;
			}
			return true;
		}

		public string ToTextDefault(byte[] data)
		{
			string value = "";
			if (data == null) {
				return "";
			}
			for (int i = 0; i < data.Length; i++) {
				value += data[i].ToString("X2");
			}
			return value;
		}

		public string ToTextTimeBox(byte[] data)
		{

			string str = ToTextDefault(data);
			if (str.Length == 4) {
				int h = int.Parse("0x" + str.Substr(0, 2));
				int m = int.Parse("0x" + str.Substr(2, 2));
				return h.ToString("D2") + ":" + m.ToString("D2");
			}
			else {
				return "";
			}
		}

		public string ToTextCal(byte[] data)
		{
			string str = ToText(data);
			if (str.Length == 8) {
				int y = int.Parse("0x" + str.Substr(0, 4));
				int m = int.Parse("0x" + str.Substr(4, 2));
				int d = int.Parse("0x" + str.Substr(6, 2));
				if ((y < 1 || m < 1) || d < 1) {
					return y.ToString("D4") + "/" + m.ToString("D2") + "/" + m.ToString("D2");
				}
				DateTime date = new DateTime(y, m, d);
				return date.ToString("yyyy/MM/dd");
			}
			else {
				return "";
			}
		}

		public string ToText(byte[] data)
		{
			if (data == null) {
				return "";
			}
			if (FieldContain.Input.GetType().Equals(typeof(UITimeBox))) {
				return ToTextTimeBox(data);
			}
			else if (FieldContain.Input.GetType().Equals(typeof(UICalBox))) {
				return ToTextCal(data);
			}
			else {
				return ToTextDefault(data);
			}
		}

		/// <summary>
		/// 値が変わった時true
		/// </summary>
		public bool Changed { get { return !FieldContain.Input.Value.Equals(ToText(GetValue)); } }
		public void GrpBoxChange(Element ele, jQueryEvent ev)
		{
			BackButton.Toggle(Changed);
			if (!Changed) {
				dpi.IsWaitRes = false;
				dpi.InputData = null;
				RemoveClass();
			}
		}

		public EventHandlerDevicePropertyInfo KadecotSet;
		public void SetGetValue(byte[] data)
		{
			RemoveClass();
			GetValue = data;
			FieldContain.Input.Value = ToText(data);
			FieldContain.Input.Change();
		}

		public UIBackButton BackButton = new UIBackButton();
	}

	public class UIBackButton : UIButton
	{
		public UIBackButton()
			: base()
		{
			base.ButtonIcon = Icon.back;
			base.Title = "元に戻す";
			base.TabIndex = -1;
			base.AddClass("text_backButton");
			base.Object.Hide();
		}
	}

	public class UIDiv : UIObject
	{
		public UIDiv()
			: base("<div>")
		{

		}
	}

	public class UILabel : UIObject
	{
		public UILabel()
			: base("<label>")
		{
		}

		public UILabel(string text)
			: this()
		{
			base.Text = text;
		}

		public UILabel(string text, string @for)
			: this(text)
		{
			base.For = @for;
		}

		public UILabel(string text, string @for, string name)
			: this(text, @for)
		{
			base.Name = name;
		}

		public UILabel(DeviceController.DevicePropertyInfo dpi)
			: this(dpi.GetText())
		{
			base.Title = dpi.PropertyInfo.valueDescription;
		}
	}

	public class UIFieldContain : UIObject
	{
		public UIFieldContain()
			: base("<div>")
		{
			base.DataRole = "fieldcontain";
		}

		public UIFieldContain(UIObject uiobject)
			: this()
		{
			base.Append(uiobject);
		}

		public UIFieldContain(UILabel label, UIObject uiobject)
			: this()
		{
			this.Label = label;
			this.Input = uiobject;
			base.Append(this.Label);
			base.Append(this.Input);
		}

		public UILabel Label;
		public UIObject Input;
	}

	public class UIGridA : UIObject
	{
		public UIGridA()
			: base("<div>")
		{
			this.Object.AddClass("ui-grid-a");
			this.Append(BlockA);
			this.Append(BlockB);
		}

		public UIBlockA BlockA = new UIBlockA();
		public UIBlockB BlockB = new UIBlockB();

		public class UIBlockA : UIObject
		{
			public UIBlockA()
				: base("<div>")
			{
				this.Object.AddClass("ui-block-a");
			}
		}

		public class UIBlockB : UIObject
		{
			public UIBlockB()
				: base("<div>")
			{
				this.Object.AddClass("ui-block-b");
			}
		}
	}

	public class UIFlipSwitch : UIObject
	{
		public UIFlipSwitch()
			: base("<select>")
		{
			base.DataRole = "flipswitch";
			//base.Append(new UIOption());
			//base.Append(new UIOption());
		}

		public UIFlipSwitch(UIOption off, UIOption on)
			: this()
		{
			this.OFF = off;
			this.ON = on;
			base.Append(this.OFF);
			base.Append(this.ON);
		}

		public UIOption ON;//{ get { return new UIOption(base.Object.Find("option").Eq(1)); } set { base.Object.Find("option").Eq(1).ReplaceWith(value.Object); } }
		public UIOption OFF;// { get { return new UIOption(base.Object.Find("option").Eq(0)); } set { base.Object.Find("option").Eq(0).ReplaceWith(value.Object); } }
	}

	public class UIButton : UIObject
	{
		public UIButton()
			: base("<a>")
		{
			this.Button = true;
			this.NoText = true;

			this.Href = "#";
			this.CornerAll = true;
		}

		public bool Button { get { return base.Object.HasClass("ui-btn"); } set { base.Object.ToggleClass("ui-btn", value); } }
		public bool NoText { get { return base.Object.HasClass("ui-btn-icon-notext"); } set { base.Object.ToggleClass("ui-btn-icon-notext", value); } }
		public bool CornerAll { get { return base.Object.HasClass("ui-corner-all"); } set { base.Object.ToggleClass("ui-corner-all", value); } }
		public Icon ButtonIcon { get { return getIcon(); } set { base.Object.ToggleClass(getClass(value), true); } }
		public string Href { get { return base.Object.GetAttribute("href"); } set { base.Object.Attribute("href", value); } }
		public enum Icon
		{
			delete,
			refresh,
			back,
		}

		private string getClass(Icon icon)
		{
			string text = "";
			if (icon == Icon.delete) {
				text = "delete";
			}
			else if (icon == Icon.refresh) {
				text = "refresh";
			}
			else if (icon == Icon.back) {
				text = "back";
			}
			return "ui-icon-" + text;
		}

		private Icon getIcon()
		{
			if (base.Object.HasClass("ui-icon-delete")) {
				return Icon.delete;
			}
			else if (base.Object.HasClass("ui-icon-refresh")) {
				return Icon.refresh;
			}
			else if (base.Object.HasClass("ui-icon-back")) {
				return Icon.back;
			}
			return Icon.delete;
		}
	}

	public class UIOption : UIObject
	{
		public UIOption()
			: base("<option>")
		{

		}

		public UIOption(string text, string value)
			: this()
		{
			base.Text = text;
			base.Value = value;
		}

		public UIOption(jQueryObject @object)
			: base(@object)
		{

		}
	}

	public class UIRadioInput : UIObject
	{
		public UIRadioInput()
			: base("<input>")
		{
			base.Object.Attribute("type", "radio");
		}

		public UIRadioInput(string id, string name)
			: this()
		{
			base.Id = id;
			base.Name = name;
		}

		public UIRadioInput(string id, string name, string value)
			: this(id, name)
		{
			base.Value = value;
		}

		public UIRadioInput(string value)
			: this()
		{
			base.Value = value;
		}
	}

	public class UILi : UIObject
	{
		public UILi()
			: base("<li>")
		{
		}
	}

	public class UIFieldSet : UIObject
	{
		public UIFieldSet()
			: base("<fieldset>")
		{
			this.ControlGroup = true;
		}

		public UIFieldSet(bool horizontal)
			: this()
		{
			this.Horizontal = horizontal;
		}

		public string CheckdValue()
		{
			return jQuery.Select("input[type='radio']:checked", base.Object).GetValue();
		}

		public void Append(RadioInput radio)
		{
			if (RadioList == null) {
				RadioList = new List<RadioInput>();
			}
			RadioList.Add(new RadioInput(radio.Label, radio.Radio));
			RadioInput last = RadioList.Last<RadioInput>();
			base.Append(last.Label);
			base.Append(last.Radio);
		}

		public List<RadioInput> RadioList;
		private bool _ControlGroup;
		public bool ControlGroup
		{
			get
			{
				return _ControlGroup;
			}
			set
			{
				if (value) {
					base.DataRole = "controlgroup";
				}
				else {
					base.DataRole = "";
				}
				_ControlGroup = value;
			}
		}

		private bool _Horizontal;
		public bool Horizontal
		{
			get { return _Horizontal; }
			set
			{
				if (value) {
					base.DataType = "horizontal";
				}
				else {
					base.DataType = "";
				}
				_Horizontal = value;
			}
		}

		public class RadioInput
		{
			public UIRadioInput Radio;
			public UILabel Label;
			public RadioInput(UILabel Label, UIRadioInput Radio)
			{
				this.Radio = Radio;
				this.Label = Label;
			}
		}
	}

	public class UIRangeInput : UIObject
	{
		public UIRangeInput()
			: base("<input>")
		{
			base.Type = "number";
			base.DataType = "range";
			this.DataHighlight = true;
			base.Keypress(SliderKeypress);
			this.Handle = true;
			base.One("slidecreate", SliderCreate);
		}

		public UIRangeInput(int min, int max)
			: this()
		{
			base.Min = min;
			base.Max = max;
			base.Maxlength = GetMaxLength();
		}

		private int GetMaxLength()
		{
			int min = 0;
			int j;
			if (Min < 0) {
				min = 1;
				j = -10;
				while (Max / j > 1) {
					min++;
					j *= 10;
				}
				min++;

			}
			int i = 1;
			j = 10;
			while (Max / j >= 1) {
				i++;
				j *= 10;
			}

			return (i > min) ? i : min;
		}

		bool DataHighlight
		{
			get { return bool.Parse(base.GetAttribute("data-highlight")); }
			set { base.Attribute("data-highlight", value.ToString()); }
		}

		public new int Value
		{
			get
			{
				if (base.Value == "") {
					this.Value = base.Min;
				}
				return int.Parse(base.Value);
			}
			set { base.Value = value.ToString(); }
		}

		private void SliderKeypress(Element ele, jQueryEvent ev)
		{
			bool number = (ev.Which >= (int)KeyPress._0 && ev.Which <= (int)KeyPress._9);

			bool BackSpace = (ev.Which == (int)KeyPress.BackSpace);
			bool Delete = (ev.Which == (int)KeyPress.Delete);
			bool minus = (ev.Which == (int)KeyPress.minus);
			bool flag = (!number && !minus) && (!BackSpace && !Delete);
			if (flag) ev.PreventDefault();
		}

		public bool Handle;
		public UIHandle SliderHandle;

		private void SliderCreate(Element ele, jQueryEvent ev)
		{
			SliderHandle = new UIHandle(ev);
			SliderHandle.Toggle(this.Handle);

			UISliderTrack SliderTrack = new UISliderTrack(ev);
			SliderTrack.PointerEvents(this.Handle);
			base.PointerEvents(this.Handle);
		}

		public class UIHandle : UIObject
		{
			public UIHandle(jQueryEvent ev)
			{
				base.Object = jQuery.Select(ev.Target).Parent().Find(".ui-slider-handle");
			}
		}

		public class UISliderTrack : UIObject
		{
			public UISliderTrack(jQueryEvent ev)
			{
				base.Object = jQuery.Select(ev.Target).Parent().Find(".ui-slider-track");
			}
		}
	}

	public class UITextInput : UIObject
	{
		public UITextInput()
			: base("<input>")
		{
			base.Type = "text";
			this.DataClearBtn = true;
		}

		public UITextInput(JsDictionary dic)
			: this()
		{
			base.Attribute(dic);
		}

		public bool DataClearBtn
		{
			get { return bool.Parse(base.GetAttribute("data-clear-btn")); }
			set
			{
				if (value) { base.Attribute("data-clear-btn", value.ToString()); }
				else { base.RemoveAttr("data-clear-btn"); };
			}
		}

		public string Placeholder { get { return base.GetAttribute("placeholder"); } set { base.Attribute("placeholder", value); } }
		public JsDictionary DataOptions
		{
			get
			{
				return Json.Parse<JsDictionary>(base.GetAttribute("data-options"));
			}
			set
			{
				base.Attribute("data-options", Json.Stringify(value));
			}
		}

		public jQueryObject Keyup(jQueryEventHandlerWithContext eventHandler)
		{
			base.Object.Keyup(eventHandler);
			return base.Object;
		}

		public jQueryObject On(string eventName, jQueryEventHandlerWithContext eventHandler)
		{
			base.Object.On(eventName, eventHandler);
			return base.Object;
		}

		public bool ReadOnly
		{
			get { return base.GetAttribute("readonly") == "readonly"; }
			set
			{
				if (value) {
					this.Attribute("readonly", "readonly");
				}
				else {
					this.RemoveAttr("readonly");
				}
			}
		}
	}

	public class UITimeBox : UITextInput
	{
		public UITimeBox()
			: base()
		{
			base.DataRole = "datebox";
			base.DataOptions = new JsDictionary("mode", "timebox");
			base.DataClearBtn = false;
			base.On("dateboxcreate", DateBoxCreate);
		}

		private void DateBoxCreate(Element ele, jQueryEvent ev)
		{
			jQuery.Select(ele).Parents().Find(".ui-icon-grid").ToggleClass("ui-icon-grid", "ui-icon-clock");
		}
	}

	public class UICalBox : UITextInput
	{
		/// <summary>年</summary>
		public int dY { get { return GetCal(0); } }

		/// <summary>月</summary>
		public int dM { get { return GetCal(1); } }

		/// <summary>日</summary>
		public int dD { get { return GetCal(2); } }

		/// <summary>年（16進4桁）</summary>
		public string xY { get { return dY.ToString("X4"); } }

		/// <summary>月(16進2桁)</summary>
		public string xM { get { return dM.ToString("X2"); } }

		/// <summary>日(16進2桁)</summary>
		public string xD { get { return dD.ToString("X2"); } }

		/// <summary>年月日（16進8桁）</summary>
		public string YYYYMMDD { get { return xY + xM + xD; } }

		/// <summary>年（10進4桁）/月(10進4桁)/日(10進4桁)</summary>
		public string yyyymmdd { get { return dY.ToString("D4") + "/" + dM.ToString("D2") + "/" + dD.ToString("D2"); } }

		public UICalBox()
			: base()
		{
			base.DataRole = "datebox";
			base.DataOptions = new JsDictionary("mode", "calbox");
			base.DataClearBtn = false;
			base.On("dateboxcreate", DateBoxCreate);
		}

		private void DateBoxCreate(Element ele, jQueryEvent ev)
		{
			jQuery.Select(ele).Parents().Find(".ui-icon-grid").ToggleClass("ui-icon-grid", "ui-icon-calendar");
		}

		private int GetCal(int i)
		{
			int x = 0;
			if (Value.Contains("/")) {
				string[] value = Value.Split("/");
				x = int.Parse(value[i]);
			}
			return x;
		}

		public class UIInputDate : UIObject
		{
			public UIInputDate(jQueryEvent ev)
			{
				base.Object = jQuery.Select(ev.Target).Parent(".ui-input-datebox");
			}
		}
	}

	public class UICollapsibleset : UIObject
	{
		public UICollapsibleset()
			: base("<div>")
		{
			base.DataRole = "collapsible";
		}

		public UICollapsibleset(JsDictionary attr, string h2)
			: this()
		{
			base.Attribute(attr);
			base.Append(new UIH(2, h2));
		}

		public UICollapsibleset(string h2)
			: this()
		{
			base.Append(new UIH(2, h2));
		}

		public bool DataInset { get { return bool.Parse(base.Object.GetAttribute("data-inset")); } set { base.Object.Attribute("data-inset", value.ToString()); } }
	}

	public class UIListview : UIObject
	{
		public UIListview()
			: base("<ul>")
		{
			base.DataRole = "listview";
		}

		public UIListview(params object[] nameValuePairs)
			: this()
		{
			base.Attribute(new JsDictionary(nameValuePairs));
		}
	}

	public class UIListviewItem : UIObject
	{
		public UIListviewItem()
			: base("<li>")
		{

		}

		public UIListviewItem(UIObject uiobject)
			: this()
		{
			base.Append(uiobject);
		}
	}

	public class UIListDivider : UIListviewItem
	{
		public UIListDivider()
			: base()
		{
			base.DataRole = "list-divider";
		}

		public UIListDivider(string text)
			: this()
		{
			base.Text = text;
		}

		public UIListDivider(UIObject uiobject)
			: this()
		{
			base.Append(uiobject);
		}

		public UIListDivider(jQueryObject @object)
			: this()
		{
			base.Append(@object);
		}
	}

	public class UIP : UIObject
	{
		public UIP()
			: base("<p>")
		{
		}

		public UIP(string text)
			: this()
		{
			base.Text = text;
		}

		public UIP(string text, string @class)
			: this(text)
		{
			base.AddClass(@class);
		}
	}

	public class UIAnchor : UIObject
	{
		public UIAnchor()
			: base("<a>")
		{

		}

		public UIAnchor(string text)
			: this()
		{
			base.Text = text;
		}

		public UIAnchor(UIObject uiobject)
			: this()
		{
			base.Append(uiobject);
		}

		public string Href { get { return base.GetAttribute("href"); } set { base.Attribute("href", value); } }
		public string Target { get { return base.GetAttribute("target"); } set { base.Attribute("target", value); } }
	}

	public class UIH : UIObject
	{
		public UIH()
			: base("<h1>")
		{
		}

		public UIH(int i)
			: base("<h" + i.ToString() + ">")
		{

		}

		public UIH(int i, string text)
			: this(i)
		{
			base.Text = text;
		}
	}

	public class UIImg : UIObject
	{
		public UIImg()
			: base("<img>")
		{

		}

		public UIImg(JsDictionary attr)
			: this()
		{
			base.Attribute(attr);
		}

		public UIImg(params object[] nameValuePairs)
			: this()
		{
			base.Attribute(new JsDictionary(nameValuePairs));
		}
	}

	public class UIPin : UILi
	{
		public UIButton ReadButton;
		public UIFieldSet UIPinMode;
		public UIRangeInput Range;
		public UIRangeInput AnalogRead;
		public UIFlipSwitch FlipSwitch;
		public int Pin;
		public UIListDivider ListDivider;
		private UIFieldSet SetUIPinMode(int i)
		{
			UIPinMode.Append(new UILabel("INPUT", "input" + i.ToString(), "pin" + i.ToString()));
			UIPinMode.Append(new UIRadioInput("input" + i.ToString(), "pin" + i.ToString(), "INPUT"));
			UIPinMode.Append(new UILabel("OUTPUT", "output" + i.ToString(), "pin" + i.ToString()));
			UIPinMode.Append(new UIRadioInput("output" + i.ToString(), "pin" + i.ToString(), "OUTPUT"));
			UIPinMode.Append(new UILabel("INPUT_PULLUP", "input_pullup" + i.ToString(), "pin" + i.ToString()));
			UIPinMode.Append(new UIRadioInput("input_pullup" + i.ToString(), "pin" + i.ToString(), "INPUT_PULLUP"));
			return UIPinMode;
		}

		private void SetUIDigital(int i)
		{
		}

		private void SetUIRange(int i)
		{
			Range = new UIRangeInput(-1, 255);
			Range.Value = 123;
		}

		private void SetAnalogRead(int i)
		{
			AnalogRead = new UIRangeInput(0, 1023);
			AnalogRead.Handle = false;
		}

		public UIPin()
			: base()
		{
			base.AddClass("pin");
			ListDivider = new UIListDivider();
			UIPinMode = new UIFieldSet(true);

			Range = new UIRangeInput();

			ReadButton = new UIButton();
			ReadButton.Title = "取得";
			ReadButton.ButtonIcon = UIButton.Icon.refresh;
		}

		public UIPin(int pin)
			: this()
		{
			this.Pin = pin;
			UIDiv Div = new UIDiv();

			Div.Append(new UIH(1, "pin" + pin.ToString()));
			Div.Append(ReadButton);
			ListDivider.Append(Div);
			SetUIPinMode(pin);
			SetUIDigital(pin);
			SetUIRange(pin);

			if (pin >= 0 && pin <= 7) {
				FlipSwitch = new UIFlipSwitch(new UIOption("LOW", "LOW"), new UIOption("HIGH", "HIGH"));
				base.Append(new UIFieldContain(UIPinMode));
				base.Append(new UIFieldContain(FlipSwitch));
				base.Append(new UIFieldContain(Range));
			}
			else if (pin >= 8 && pin <= 13) {
				FlipSwitch = new UIFlipSwitch(new UIOption("LOW", "LOW"), new UIOption("HIGH", "HIGH"));
				base.Append(new UIFieldContain(UIPinMode));
				base.Append(new UIFieldContain(FlipSwitch));
			}
			else if (pin >= 14 && pin <= 19) {
				SetAnalogRead(pin);
				base.Append(new UIFieldContain(AnalogRead));
			}
		}

		public Element[] GetElements()
		{
			Element[] elements = new Element[2];
			elements[0] = ListDivider.Element;
			elements[1] = base.Element;
			return elements;
		}

		public void SetPinModeChange(EventHandlerFieldSet eventhandler)
		{
			if (UIPinMode != null) {
				UIPinMode.Change((elem, ev) => { eventhandler(UIPinMode, Pin); });
			}
		}

		public void SetDigitalChange(EventHandlerFlipSwitch eventhandler)
		{
			if (FlipSwitch != null) {
				FlipSwitch.Change((elem, ev) => { eventhandler(ReadButton, FlipSwitch, Pin); });
			}
		}

		public void SetReadButtonClick(EventHandlerFlipSwitch eventhandler)
		{
			if (FlipSwitch != null) {
				ReadButton.Click((elem, ev) => { eventhandler(ReadButton, FlipSwitch, Pin); });
			}
		}

		public void SetAnalogWriteChange(EventHandlerRange eventhandler)
		{
			if (Range != null) {
				Range.Change((elem, ev) => { eventhandler(Range, Pin); });
			}
		}

		public void SetAnalogReadChange(EventHandlerRange eventhandler)
		{
			if (AnalogRead != null) {
				ReadButton.Click((elem, ev) => { eventhandler(AnalogRead, Pin); });
			}
		}
	}

	public delegate void EventHandlerFieldSet(UIFieldSet fieldcontain, int pin);
	public delegate void EventHandlerFlipSwitch(UIButton button, UIFlipSwitch fieldcontain, int pin);
	public delegate void EventHandlerDevicePropertyInfo(DeviceController.DevicePropertyInfo dpi, byte[] data);
	public delegate void EventHandlerRange(UIRangeInput fieldcontain, int pin);

	public class UIDeviceController
	{
		public DeviceController DeviceController;
		public class UIDeviceInfo
		{
			protected CtrlUI Page;
			public DeviceController.DeviceInfo di = null;
			Element[] Elements;

			protected void UIDeviceSet(DeviceController.DeviceInfo di, CtrlUI Page)
			{
				this.di = di;
				this.Page = Page;
				if (((WampApiKadecotDevice)di.Data).protocol == "arduino") {
					Elements = di.SetPin(Page);
				}
				else {
					Elements = di.SetEchonetlite(Page);
				}
			}

			public jQueryObject GetPropertys()
			{
				return jQuery.FromElements(Elements);
			}

			public void GetData()
			{
				foreach (DeviceController.DevicePropertyInfo dpi in di.Properties) {
					if (dpi.Data != null) {
						dpi.GetValue(dpi.Data);
					}
				}
			}

			public bool IsWait()
			{
				bool flag = false;

				if (di != null && di.Properties != null) {
					foreach (DeviceController.DevicePropertyInfo dpi in di.Properties) {
						if (dpi != null && dpi.IsWaitRes) {
							flag = true;
						}
					}
				}

				return flag;
			}

			public Element[] SetEchonetlite(CtrlUI ctrlui)
			{
				Element[] ele = new Element[di.Properties.Count];
				int i = 0;
				foreach (DeviceController.DevicePropertyInfo dpi in di.Properties) {
					dpi.SetProperty(dpi);
					dpi.SetName(di);
					dpi.SetKadecotGet(Page.KadecotGet);
					dpi.SetKadecotSet(Page.KadecotSet);
					ele[i++] = dpi.GetElement();
				}
				return ele;
			}

			public Element[] SetPin(CtrlUI ctrlui)
			{
				Page = ctrlui;
				Element[] ele = new Element[20];
				Element[] element;
				int j = 0;
				for (int i = 0; i <= 19; i++) {
					element = SetPin(i, Page);
					ele[j++] = element[0];
					ele[j++] = element[1];
				}
				return ele;
			}

			public Element[] SetPin(int i, CtrlUI Page)
			{
				UIPin uiPin = new UIPin(i);
				uiPin.SetPinModeChange(Page.pinMode_Change);
				uiPin.SetDigitalChange(Page.digital_Change);
				uiPin.SetReadButtonClick(Page.DigitalRead);
				uiPin.SetAnalogWriteChange(Page.analogWrite_Change);
				uiPin.SetAnalogReadChange(Page.analogRead);
				return uiPin.GetElements();
			}
		}

		public jQueryObject GetUINodeList(jQueryEventHandlerWithContextDi listitem_click)
		{
			UINodeList nodelist = new UINodeList(DeviceController.NodeList, listitem_click);

			return nodelist.GetNodeList();
		}

		public delegate void jQueryEventHandlerWithContextDi(Element elem, jQueryEvent e, DeviceController.DeviceInfo node);

		public class UIDevicePropertyInfo
		{
			private UIGrpBox Grpbox;

			public Element GetElement()
			{
				return Grpbox.Object.GetElement(0);
			}

			public void SetName(DeviceController.DeviceInfo di)
			{
				string nameid = di.GetObjectID() + dpi.GetPropertyCode("X2");
				Grpbox.SetName(nameid);
			}

			public string ErrorText { get { return Grpbox.ErrorText; } set { Grpbox.ErrorText = value; } }

			public int GetMaxLength()
			{
				int len = 2 * ((dpi.PropertyInfo == null) ? dpi.Size : (((dpi.PropertyInfo.arrayCount == 0) ? 1 : dpi.PropertyInfo.arrayCount) * dpi.PropertyInfo.size));
				return len;
			}

			public void SetProperty(DeviceController.DevicePropertyInfo dpi)
			{
				this.dpi = dpi;
				Grpbox = new UIGrpBox(dpi);
			}

			public void SetKadecotGet(EventHandlerPropertyInfo KadecotGet)
			{
				DeviceController.DevicePropertyInfo opeSt = null;

				//現在時刻設定
				if (dpi.PropertyCode == 0x97) {
				}
				else if (dpi.PropertyCode == 0x98) {

				}
				else if (this.dpi.InputTypes.Count == 0) {

				}
				else if (dpi.PropertyCode == 0x80 && dpi.InputTypes.Count == 1 && dpi.InputTypes[0].Mode == PropertyInputMode.Select) {

				}

				else {
					foreach (PropertyInputInfo pii in dpi.InputTypes) {
						if (dpi.PropertyCode == 0x80)
							opeSt = dpi;
					}
					if (opeSt != null)
						KadecotGet(opeSt);
				}
			}

			public void SetKadecotSet(EventHandlerDevicePropertyInfo KadecotSet)
			{
				Grpbox.KadecotSet = KadecotSet;
			}

			private DeviceController.DevicePropertyInfo dpi;

			public delegate void EventHandlerPropertyInfo(DeviceController.DevicePropertyInfo dpi);

			public void GetValue(byte[] data)
			{
				Grpbox.SetGetValue(data);
			}
		}
	}

	public class UINodeList
	{
		public UINodeList(IEnumerable<DeviceController.NodeInfo> NodeList, UIDeviceController.jQueryEventHandlerWithContextDi listitem_click)
		{
			info = new UIDiv();
			string identifer = "";
			foreach (var node in NodeList) {

				var profile = node.Profile;

				if (jQuery.Select("#" + "cl_node_" + identifer).Length > 0)
					continue;
				UICollapsibleset col = new UICollapsibleset(node.Data.ToString());
				col.Id = "cl_node_" + identifer;
				col.DataInset = true;
				info.Append(col);
				UIListview listview = new UIListview("id", "lv_node_" + identifer);

				col.Append(listview);
				foreach (var item in node.Devices) {

					if (item.Profile)
						continue;

					identifer = item.GetObjectID();
					if (jQuery.Select("#" + "li_device_" + identifer).Length > 0)
						continue;
					UIListviewItem listitem = new UIListviewItem();
					listitem.Id = "li_device_" + identifer;
					listitem.Click((ele, ev) => { listitem_click(ele, ev, item); });
					listview.Append(listitem);


					UIAnchor anchor = new UIAnchor();
					anchor.Href = "#";
					anchor.Title = ((WampApiKadecotDevice)item.Data).description;
					anchor.Text = ((WampApiKadecotDevice)item.Data).nickname;
					anchor.Click((ele, ev) => { listitem_Click(listitem, item); });

					listitem.Append(anchor);
				}
			}
		}

		private UIDiv info;

		public jQueryObject GetNodeList()
		{
			return info.Children();
		}

		/// <summary>
		/// "#li_device_XXXXXX"をクリック
		/// </summary>
		/// <param name="elem"></param>
		/// <param name="ev"></param>
		private void listitem_Click(UIListviewItem listitem, DeviceController.DeviceInfo item)
		{
			//選択中クラスの追加
			jQuery.Select(".li-select").RemoveClass("li-select");
			listitem.AddClass("li-select");
		}
	}

	public static class UIEventNames
	{
		public static string CurrentNodeChange = "currentnode.change";
		public static string LoadingShow = "loading.show";
		public static string LoadingHide = "loading.hide";
		/// <summary>
		/// 機器検索開始
		/// </summary>
		public static string SearchStart = "search.start";
		/// <summary>
		/// 機器検索終了
		/// </summary>
		public static string SearchEnd = "search.end";
		/// <summary>
		/// 機器検索エラー
		/// </summary>
		public static string SearchError = "search.error";
		/// <summary>
		/// 入力画面作成開始
		/// </summary>
		public static string PropertyWriteStart = "propertywrite.start";
		/// <summary>
		/// 入力画面作成終了
		/// </summary>
		public static string PropertyWriteEnd = "propertywrite.end";
	}

	public static class UIEventHandler
	{
		public static void LoadingShow(jQueryEvent ev)
		{
			jQueryMobile.Loading("show");
		}

		public static void LoadingHide(jQueryEvent ev)
		{
			jQueryMobile.Loading("hide");
		}
	}

	public enum KeyPress
	{
		Delete = 0,
		BackSpace = 8,
		minus = 45,
		dot = 46,
		slash = 47,
		_0 = 48,
		_1 = 49,
		_2 = 50,
		_3 = 51,
		_4 = 52,
		_5 = 53,
		_6 = 54,
		_7 = 55,
		_8 = 56,
		_9 = 57,
		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		a = 97,
		b = 98,
		c = 99,
		d = 100,
		e = 101,
		f = 102,
	}
}
