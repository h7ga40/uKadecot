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
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Xml;
using control;

namespace ctrlui
{
	public enum PropertyInputMode
	{
		Unknown,
		Select,
		Range,
	}

	public class PropertyInputInfo
	{
		public PropertyInputMode Mode;
	}

	public class PropertySelectInput : PropertyInputInfo
	{
		public Dictionary<long, string> Option = new Dictionary<long, string>();

		public PropertySelectInput()
		{
			Mode = PropertyInputMode.Select;
		}
	}

	public class PropertyRangeInput : PropertyInputInfo
	{
		public int Maximum;
		public int Minimum;
		public string MinDescrption;
		public string MaxDescrption;

		public PropertyRangeInput()
		{
			Mode = PropertyInputMode.Range;
		}
	}

	public class DeviceController : UIDeviceController
	{

		public class NodeInfo
		{
			private ENOD_ID m_NodeId;
			private DeviceInfo m_Profile;
			private List<DeviceInfo> m_Devices = new List<DeviceInfo>();

			public NodeInfo(ENOD_ID nodeId, DeviceInfo profile)
			{
				m_NodeId = nodeId;
				m_Profile = profile;
			}

			public ENOD_ID NodeId { get { return m_NodeId; } }

			public DeviceInfo Profile { get { return m_Profile; } }

			public List<DeviceInfo> Devices { get { return m_Devices; } }

			public object Data { get; set; }

		}
		public class DeviceInfo : UIDeviceInfo
		{
			private JsonClassInfo m_ClassInfo;
			private bool m_Profile;
			private byte m_ClassGroupCode;
			private byte m_ClassCode;
			private byte m_InstanceCode;
			private List<DevicePropertyInfo> m_Properties = new List<DevicePropertyInfo>();
			private int m_EObjId;

			public DeviceInfo(JsonClassInfo eci, bool profile, byte x1, byte x2, byte x3, int eobjid)
			{
				m_ClassInfo = eci;
				m_Profile = profile;
				m_ClassGroupCode = x1;
				m_ClassCode = x2;
				m_InstanceCode = x3;
				m_EObjId = eobjid;
			}

			public DeviceInfo(JsonClassInfo eci, WebApiObjectInfo dev)
				: this(eci, dev.profile, dev.x1, dev.x2, dev.x3, dev.eobjid)
			{
			}

			public DeviceInfo(JsonClassInfo eci, DevicePropertyInfo[] prps)
			{
				m_ClassInfo = eci;
				m_Properties.AddRange(prps);
			}

			public JsonClassInfo ClassInfo { get { return m_ClassInfo; } }

			public bool Profile { get { return m_Profile; } }

			public byte ClassGroupCode { get { return m_ClassGroupCode; } }

			public byte ClassCode { get { return m_ClassCode; } }

			public byte InstanceCode { get { return m_InstanceCode; } }

			public List<DevicePropertyInfo> Properties { get { return m_Properties; } }

			public int EObjId { get { return m_EObjId; } }

			public object Data { get; set; }

			public string GetText()
			{
				if (m_ClassInfo != null)
					return m_ClassInfo.description;
				else
					return String.Format("{0:X2}{1:X2}{2:X2}", m_ClassGroupCode, m_ClassCode, m_InstanceCode);
			}
			/// <summary>
			/// プロパティ値を採用
			/// </summary>
			/// <param name="epc">番号</param>
			/// <param name="edt">入力値</param>
			internal void SetPropertyData(byte epc, byte[] edt)
			{
				DevicePropertyInfo dpi = m_Properties.FirstOrDefault(p => p.PropertyCode == epc);
				if (dpi == null)
					return;

				dpi.SetPropertyData(edt);
			}
			internal void AdoptInputData(byte epc)
			{
				DevicePropertyInfo dpi = m_Properties.FirstOrDefault(p => p.PropertyCode == epc);
				if (dpi == null)
					return;

				dpi.AdoptInputData();
			}
			public string GetObjectID()
			{
				string text = ClassGroupCode.ToString("X2")
				 + ClassCode.ToString("X2")
				 + InstanceCode.ToString("X2");
				return text;
			}
			public void SetUIDevice(CtrlUI page)
			{
				base.UIDeviceSet(this, page);
			}
		}

		public delegate void GetClassInfoCallback(JsonClassInfo eclass);
		public delegate void GetClassInfoHandler(byte x1, byte x2, GetClassInfoCallback cb);
		public delegate void CreateCallback(DeviceController dc);

		public class DevicePropertyInfo : UIDevicePropertyInfo
		{
			private JsonPropertyInfo m_PropertyInfo;
			private byte m_PropertyCode;
			private EPC_FLAG m_Flag;
			private byte m_Size;
			private byte[] m_Data;
			List<PropertyInputInfo> m_InputTypes;
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="epi"></param>
			/// <param name="epc"></param>
			/// <param name="flag"></param>
			/// <param name="size"></param>
			public DevicePropertyInfo(JsonPropertyInfo epi, byte epc, EPC_FLAG flag, byte size)
			{
				m_PropertyInfo = epi;
				m_PropertyCode = epc;
				m_Flag = flag;
				m_Size = size;
				//base.SetProperty(this);
			}
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="epi"></param>
			/// <param name="prop"></param>
			public DevicePropertyInfo(JsonPropertyInfo epi, WebApiPropertyInfo prop)
				: this(epi, prop.epc, prop.flag, (byte)epi.size)
			{

			}
			public JsonPropertyInfo PropertyInfo
			{
				get { return m_PropertyInfo; }
			}
			public byte PropertyCode
			{
				get { return m_PropertyCode; }
			}
			public string GetPropertyCode(string format)
			{
				return PropertyCode.ToString(format);
			}
			/// <summary>
			/// 可変長データ
			/// </summary>
			/// <returns>true:可変長データ</returns>
			public bool IsValiable()
			{
				return (PropertyInfo == null) ? true : PropertyInfo.access.Contains("VARIABLE");
			}
			/// <summary>
			/// アクセスルール
			/// </summary>
			public EPC_FLAG Flag
			{
				get { return m_Flag; }
			}
			public byte Size
			{
				get { return m_Size; }
			}
			/// <summary>
			/// 表示名称を返します
			/// </summary>
			/// <returns></returns>
			public string GetText()
			{
				if (m_PropertyInfo != null)
					return m_PropertyInfo.description;
				else
					return String.Format("{0:X2}", m_PropertyCode);
			}
			/// <summary>
			/// 入力値
			/// </summary>
			public byte[] InputData { get; set; }
			/// <summary>
			/// プロパティのデータ
			/// </summary>
			public byte[] Data { get { return m_Data; } }
			/// <summary>
			/// 書き込み待ち状態
			/// </summary>
			public bool IsWaitRes { get; set; }
			public List<PropertyInputInfo> InputTypes { get { return m_InputTypes; } }
			/// <summary>
			/// プロパティ値を採用したとき、値をDataに入れる。
			/// </summary>
			/// <param name="edt"></param>
			public void SetPropertyData(byte[] edt)
			{
				m_Data = edt;
			}
			public void AdoptInputData()
			{
				m_Data = InputData;
				InputData = null;
			}
			/// <summary>
			/// InputTypesを設定
			/// </summary>
			public void PrepareInputInfo()
			{
				if (m_InputTypes != null)
					return;

				m_InputTypes = new List<PropertyInputInfo>();

				JsonPropertyInfo epi = m_PropertyInfo;
				if ((epi == null) || !epi.primitive)
					return;

				ValueRange valueRange = ValueRange.Parse(epi.valueDescription, epi);
				if (!String.IsNullOrEmpty(epi.initialValue))
					valueRange.InitailValue = epi.initialValue;

				if (valueRange.Values.Count > 0) {
					PropertySelectInput pii;
					pii = new PropertySelectInput();
					foreach (var option in valueRange.Values)
						pii.Option.Add(option.Val, option.Disp);
					m_InputTypes.Add(pii);
				}
				foreach (var range in valueRange.Ranges) {
					PropertyRangeInput pii;
					pii = new PropertyRangeInput();
					pii.Minimum = (int)range.Min;
					pii.Maximum = (int)range.Max;
					pii.MinDescrption = range.MinDisp;
					pii.MaxDescrption = range.MaxDisp;
					m_InputTypes.Add(pii);
				}
			}

		}

		public static GetClassInfoHandler GetClassInfo;
		private List<NodeInfo> m_NodeList = new List<NodeInfo>();

		public DeviceController(List<NodeInfo> nodeList)
		{
			m_NodeList = nodeList;
			base.DeviceController = this;
		}

		public IEnumerable<NodeInfo> NodeList { get { return m_NodeList; } }
		/// <summary>
		/// プロパティ値を設定
		/// </summary>
		/// <param name="response"></param>
		/// <returns></returns>
		public bool RecvResponse(WebApiEchonetMessage response)
		{
			NodeInfo ni;
			DeviceInfo di;

			GetDevice(response.sender, response.seoj, out ni, out di);
			if (ni == null) {
				Debug.WriteLine("ni == null");
				return false;
			}
			if (di == null) {
				Debug.WriteLine("di == null");
				return false;
			}
			foreach (var prop in response.properties) {
				if (prop.edt.Length > 0) {
					//プロパティ値を採用
					di.SetPropertyData(prop.epc, prop.GetEdt());
				}
				else {
					// プロパティ値書き込み応答の場合
					if (response.esv == /*ESV_SET_RES*/0x71) {
						// 入力値を採用
						di.AdoptInputData(prop.epc);
					}
				}
			}

			return true;
		}

		public void RecvResponse(DeviceInfo di, byte esv, byte epc, byte[] edt)
		{
			if (edt.Length > 0) {
				// プロパティ値を設定
				di.SetPropertyData(epc, edt);
			}
			else {
				// プロパティ値書き込み応答の場合
				if (esv == /*ESV_SET_RES*/0x71) {
					// 入力値を採用
					di.AdoptInputData(epc);
				}
			}
		}
		/// <summary>
		/// 機器を取得
		/// </summary>
		/// <param name="nodid"></param>
		/// <param name="eoj"></param>
		/// <param name="nod"></param>
		/// <param name="obj"></param>
		private void GetDevice(ENOD_ID nodid, T_ECN_EOJ eoj, out NodeInfo nod, out DeviceInfo obj)
		{
			obj = null;
			nod = FindNode(nodid);

			if (nod != null) {
				// ノードプロファイルの場合
				if ((eoj.x1 == T_ECN_EOJ.X1_PROFILE)
					&& (eoj.x2 == T_ECN_EOJ.X2_NODE_PROFILE)) {
					obj = nod.Profile;
				}
				// 機器オブジェクトの場合
				else {
					obj = FindDevice(nod, eoj);
				}
			}
		}
		/// <summary>
		///ノードを取得
		/// </summary>
		/// <param name="enodid"></param>
		/// <returns></returns>
		private NodeInfo FindNode(ENOD_ID enodid)
		{
			NodeInfo ni = null;

			switch (enodid) {
			case ENOD_ID.MULTICAST_ID:
			case ENOD_ID.LOCAL_ID:
				ni = m_NodeList[0];
				break;
			default:
				ni = m_NodeList.FirstOrDefault(p => p.NodeId == enodid);
				break;
			}

			return ni;
		}

		private DeviceInfo FindDevice(NodeInfo ni, T_ECN_EOJ eoj)
		{
			foreach (DeviceInfo di in ni.Devices) {
				if ((di.ClassGroupCode == eoj.x1)
					&& (di.ClassCode == eoj.x2)
					&& (di.InstanceCode == eoj.x3))
					return di;
			}

			return null;
		}
	}
}
