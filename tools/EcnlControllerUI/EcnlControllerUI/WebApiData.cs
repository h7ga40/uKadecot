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
using System.Serialization;
using System.Text;
using System.Web;

namespace ctrlui
{
	public class WebApiPropertyInfo
	{
		public byte epc;
		public EPC_FLAG flag;

		public WebApiPropertyInfo(dynamic prop)
		{
			epc = prop.epc;
			flag = EPC_FLAG.NONE;
			string temp = prop.flag;
			if (temp.Contains("RULE_ANNO"))
				flag |= EPC_FLAG.RULE_ANNO;
			if (temp.Contains("RULE_SET"))
				flag |= EPC_FLAG.RULE_SET;
			if (temp.Contains("RULE_GET"))
				flag |= EPC_FLAG.RULE_GET;
			if (temp.Contains("ANNOUNCE"))
				flag |= EPC_FLAG.ANNOUNCE;
			if (temp.Contains("VARIABLE"))
				flag |= EPC_FLAG.VARIABLE;
		}
	}

	public class WebApiObjectInfo
	{
		public bool profile;
		public byte x1;
		public byte x2;
		public byte x3;
		public int eobjid;
		public int enodid;
		public List<WebApiPropertyInfo> properties = new List<WebApiPropertyInfo>();

		public WebApiObjectInfo(int eobjid, dynamic obj, bool profile)
		{
			this.eobjid = eobjid;
			this.profile = profile;
			x1 = obj.x1;
			x2 = obj.x2;
			x3 = obj.x3;
			enodid = obj.enodid;

			foreach (var prop in (dynamic[])obj.properties) {
				properties.Add(new WebApiPropertyInfo(prop));
			}
		}
	}

	public class WebApiPropertyCode
	{
		public byte epc;

		public WebApiPropertyCode(dynamic prop, bool dummy)
		{
			epc = prop.epc;
		}
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="epc">PropertyCode</param>
		public WebApiPropertyCode(byte epc)
		{
			this.epc = epc;
		}
	}

	public class WebApiPropertyData
	{
		public byte epc;
		public string edt;

		public WebApiPropertyData(dynamic prop)
		{
			byte[] _edt;

			epc = prop.epc;

			string text = prop.edt;

			if (text.Length > 0) {
				_edt = new byte[text.Length / 2];

				for (int i = 0, j = 0; j < text.Length; j += 2) {

					_edt[i++] = Byte.Parse(text.Substring(j, 2), 16);
				}
			}
			else {
				_edt = new byte[0];
			}

			edt = SetEdt(_edt);
		}

		public WebApiPropertyData(byte epc, byte[] edt)
		{
			this.epc = epc;
			this.edt = SetEdt(edt);
		}

		private string SetEdt(byte[] _edt)
		{
			var sb = new StringBuilder();

			foreach (var b in _edt) {
				sb.Append(b.ToString("X2"));
			}

			return sb.ToString();
		}

		internal byte[] GetEdt()
		{
			byte[] result = new byte[edt.Length / 2];

			for (int i = 0, j = 0; j < edt.Length; i++, j += 2) {
				result[i] = Byte.Parse(edt.Substring(j, 2), 16);
			}
			return result;
		}
	}

	public class WebApiEchonetMessage
	{
		public byte esv;
		public ENOD_ID sender;
		public T_ECN_EOJ seoj = new T_ECN_EOJ(new byte[3], 0);
		public T_ECN_EOJ deoj = new T_ECN_EOJ(new byte[3], 0);
		public List<WebApiPropertyData> properties = new List<WebApiPropertyData>();

		public WebApiEchonetMessage(dynamic esv)
		{
			this.esv = esv.esv;
			sender = (ENOD_ID)esv.sender;
			string sseoj = esv.seoj;
			seoj.x1 = Byte.Parse(sseoj.Substring(0, 2), 16);
			seoj.x2 = Byte.Parse(sseoj.Substring(2, 2), 16);
			seoj.x3 = Byte.Parse(sseoj.Substring(4, 2), 16);

			foreach (var prop in (dynamic[])esv.properties) {
				properties.Add(new WebApiPropertyData(prop));
			}
		}
	}

	public class WebApiGrsaguraMessage
	{
		public int pin;
		public string value;
		public WebApiGrsaguraMessage(dynamic esv)
		{
			this.pin = esv.pin;
			if (((object)esv.value).GetType() == typeof(double)) {
				this.value = ((double)esv.value).ToString();
			}
			else {
				this.value = esv.value;
			}
		}
	}

	public class WebApiSet
	{
		public ENOD_ID deojid;
		public List<WebApiPropertyData> properties = new List<WebApiPropertyData>();

		public WebApiSet()
		{
		}

		public WebApiSet(dynamic esv)
		{
			deojid = (ENOD_ID)esv.deojid;

			foreach (var prop in (dynamic[])esv.properties) {
				properties.Add(new WebApiPropertyData(prop));
			}
		}

		public void AddEdt(byte epc, byte[] edt)
		{
			properties.Add(new WebApiPropertyData(epc, edt));
		}

		public string GetJson()
		{
			return "{\"esv_set\":" + Json.Stringify(this) + "}";
		}
		/// <summary>
		/// "{deviceId: }"を返します。
		/// </summary>
		/// <returns></returns>
		public string GetOption()
		{
			return "{\"deviceId\":" + ((int)deojid).ToString() + "}";
		}
		/// <summary>
		/// json形式にしたオブジェクト文字をかえします
		/// </summary>
		/// <returns></returns>
		public string GetArguments()
		{
			return Json.Stringify(this);
		}
	}

	/// <summary>
	/// WebApiから値を取得するクラス
	/// </summary>
	public class WebApiGet
	{
		/// <summary>
		/// deojid
		/// </summary>
		public ENOD_ID deojid;
		public List<WebApiPropertyCode> properties = new List<WebApiPropertyCode>();

		/// <summary>
		/// コンストラクタ
		/// </summary>
		public WebApiGet()
		{
		}

		public WebApiGet(dynamic esv)
		{
			deojid = (ENOD_ID)esv.deojid;

			foreach (var prop in (dynamic[])esv.properties) {
				properties.Add(new WebApiPropertyCode(prop, true));
			}
		}

		/// <summary>
		/// WebApiGet.propertiesに追加
		/// </summary>
		/// <param name="epc"></param>
		public void AddEpc(byte epc)
		{
			properties.Add(new WebApiPropertyCode(epc));
		}

		public string GetJson()
		{
			return "{\"esv_get\":" + Json.Stringify(this) + "}";
		}

		/// <summary>
		/// deviceIdを返します。
		/// </summary>
		/// <returns></returns>
		public string GetOption()
		{
			return "{\"deviceId\":" + ((int)deojid).ToString() + "}";
		}

		/// <summary>
		/// Json形式の文字列を返します
		/// </summary>
		/// <returns></returns>
		public string GetArguments()
		{
			return Json.Stringify(this);
		}
	}

	public class WebApiSetGet
	{
		public ENOD_ID deojid;
		public List<WebApiPropertyData> setProperties = new List<WebApiPropertyData>();
		public List<WebApiPropertyCode> getProperties = new List<WebApiPropertyCode>();

		public WebApiSetGet()
		{
		}

		public WebApiSetGet(dynamic esv)
		{
			deojid = (ENOD_ID)esv.deojid;

			foreach (var prop in (dynamic[])esv.properties) {
				getProperties.Add(new WebApiPropertyCode(prop, true));
			}
		}

		public void AddEdt(byte epc, byte[] edt)
		{
			setProperties.Add(new WebApiPropertyData(epc, edt));
		}

		public void AddEpc(byte epc)
		{
			getProperties.Add(new WebApiPropertyCode(epc));
		}

		public string GetJson()
		{
			return "{\"esv_set_get\":" + Json.Stringify(this) + "}";
		}

		public string GetOption()
		{
			return "{\"deviceId\":" + ((int)deojid).ToString() + "}";
		}

		public string GetArguments()
		{
			return Json.Stringify(this);
		}
	}

	public class WampApiKadecotDevice
	{
		public string description;
		public int deviceId;
		public string deviceType;
		/// <summary>
		/// ipアドレス
		/// </summary>
		public string ip_addr;
		public string nickname;
		public string protocol;
		public bool status;

		public WampApiKadecotDevice(dynamic doc)
		{
			description = (string)doc.description;
			deviceId = (int)doc.deviceId;
			deviceType = (string)doc.deviceType;
			ip_addr = (string)doc.ip_addr;
			nickname = (string)doc.nickname;
			protocol = (string)doc.protocol;
			status = (bool)doc.status;
		}
	}

	public class WampApiKadecotSet
	{
		public int deviceId;
		public string propertyName;
		public byte[] propertyValue;

		public WampApiKadecotSet(int deviceId, string propertyName, byte[] propertyValue)
		{
			this.deviceId = deviceId;
			this.propertyName = propertyName;
			this.propertyValue = propertyValue;
		}

		internal string GetOption()
		{
			return "{\"deviceId\":" + ((int)deviceId).ToString() + "}";
		}

		internal string GetArguments()
		{
			return "{\"propertyName\":\"" + propertyName + "\",\"propertyValue\":" + Json.Stringify(propertyValue) + "}";
		}
	}

	public class WampApiKadecotGet
	{
		public int deviceId;
		public string propertyName;

		public WampApiKadecotGet(int deviceId, string propertyName)
		{
			this.deviceId = deviceId;
			this.propertyName = propertyName;
		}

		internal string GetOption()
		{
			return "{\"deviceId\":" + ((int)deviceId).ToString() + "}";
		}

		internal string GetArguments()
		{
			return "{\"propertyName\":\"" + propertyName + "\"}";
		}
	}

	public class WampApiKadecotRes
	{
		public string propertyName;
		public byte[] propertyValue;

		public WampApiKadecotRes(dynamic item)
		{
			this.propertyName = (string)item.propertyName;
			if (((object)item).GetType().GetProperty("propertyValue") != null) {
				this.propertyValue = (byte[])item.propertyValue;
			}
			else {
				this.propertyValue = new byte[0];
			}
		}
	}
}
