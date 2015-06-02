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
using System.Net.WebSockets;
using System.Runtime.Serialization.Json;
using System.Threading;
using System.Threading.Tasks;
using System.Web;
using System.Web.WebSockets;
using System.Xml;
using Codeplex.Data;

namespace EcnlCtrlUI
{
	/// <summary>
	/// webapi の概要の説明です
	/// </summary>
	public class webapi : IHttpHandler
	{
		public void ProcessRequest(HttpContext context)
		{
			if (context.IsWebSocketRequest) {
				WebSocketHandler handler = new WebSocketHandler();
				context.AcceptWebSocketRequest(handler.WebSocketRequest);
			}
			else if (String.Compare(context.Request.RequestType, "POST", true) == 0) {
				var json = DynamicJson.Parse(context.Request.InputStream);

				context.Response.ContentType = "application/json";

				if (json.IsArray) {
					WebSocketHandler handler = new WebSocketHandler();
					handler.RecvMessage((dynamic[])json, (jsonMsg) => {
						context.Response.Write("  " + jsonMsg);
					});
				}
			}
			else {
				context.Response.ContentType = "text/plain";
				context.Response.Write("Hello World!!");
			}
		}

		public bool IsReusable
		{
			get
			{
				return false;
			}
		}
	}

	class WebSocketHandler
	{
		public delegate void SendHandler(string jsonMsg);

		private static List<WebSocketHandler> connectedHandlers = new List<WebSocketHandler>();
		private WebSocket webSocket;
		private SendHandler sendCallback;

		public async Task WebSocketRequest(AspNetWebSocketContext context)
		{
			connectedHandlers.Add(this);

			webSocket = context.WebSocket;
			ArraySegment<byte> buf = new ArraySegment<byte>(new byte[2048]);
			while (true) {
				WebSocketReceiveResult res =
					await webSocket.ReceiveAsync(buf, CancellationToken.None);

				if (res.MessageType == WebSocketMessageType.Close) {
					// Close Message
					connectedHandlers.Remove(this);
					await webSocket.CloseOutputAsync(WebSocketCloseStatus.NormalClosure,
						"close", CancellationToken.None);
					break;
				}
				else if (res.MessageType == WebSocketMessageType.Text) {
					// Text Message
					System.IO.MemoryStream ms = new System.IO.MemoryStream(buf.Array, 0, res.Count);
					var json = DynamicJson.Parse(ms);

					if (json.IsArray) {
						RecvMessage((dynamic[])json, SendWebSocket);
					}
				}
			}
		}
		public void RecvMessage(dynamic[] wamp, SendHandler sendCallback)
		{
			this.sendCallback = sendCallback;

			switch ((int)wamp[0]) {
			case 1:
				OnHelow(wamp);
				break;
			case 32:
				OnSubscribe(wamp);
				break;
			case 48:
				OnCall(wamp);
				break;
			}
		}

		private void OnHelow(dynamic[] wamp)
		{
			string RealmUri = wamp[1];
			dynamic Details = wamp[2];

			sendCallback(String.Format("[2,123,{0}]", Details.ToString()));
		}

		private void OnSubscribe(dynamic[] wamp)
		{
			long RequestId = (long)wamp[1];
			dynamic Options = wamp[2];
			string TopicUri = wamp[3];

			if (TopicUri == "com.sonycsl.kadecot.echonetlite.topic.GeneralLighting.OperationStatus") {
				sendCallback(String.Format("[33,{0},789]", RequestId));
			}
		}

		private void OnCall(dynamic[] wamp)
		{
			long requestId = (long)wamp[1];
			dynamic options = wamp[2];
			string procedureUri = wamp[3];
			dynamic[] arguments = (wamp.Length <= 4) ? null : (dynamic[])wamp[4];
			dynamic argumentsKw = (wamp.Length <= 5) ? null : wamp[5];

			switch (procedureUri) {
			case "com.sonycsl.kadecot.provider.procedure.getDeviceList":
				GetDeviceList(requestId);
				break;
			case "com.sonycsl.kadecot.echonetlite.procedure.set":
				KadecotSet(requestId, (int)options.deviceId, argumentsKw);
				break;
			case "com.sonycsl.kadecot.echonetlite.procedure.get":
				KadecotGet(requestId, (int)options.deviceId, argumentsKw);
				break;
			case "jp.toppers.ecnl.procedure.getDeviceInfo":
				GetDeviceInfo(requestId, (int)options.deviceId);
				break;
			case "jp.toppers.ecnl.procedure.set":
				EsvSet(requestId, (int)options.deviceId, argumentsKw);
				break;
			case "jp.toppers.ecnl.procedure.get":
				EsvGet(requestId, (int)options.deviceId, argumentsKw);
				break;
			case "jp.toppers.ecnl.procedure.setget":
				EsvSetGet(requestId, (int)options.deviceId, argumentsKw);
				break;
			case "com.sonycsl.kadecot.arduino.pinMode":
				PinMode(requestId, argumentsKw);
				break;
			case "com.sonycsl.kadecot.arduino.digitalWrite":
				DigitalWrite(requestId, argumentsKw);
				break;
			case "com.sonycsl.kadecot.arduino.digitalRead":
				DigitalRead(requestId, argumentsKw);
				break;
			case "com.sonycsl.kadecot.arduino.analogRead":
				AnalogRead(requestId, argumentsKw);
				break;
			case "com.sonycsl.kadecot.arduino.analogWrite":
				AnalogWrite(requestId, argumentsKw);
				break;
			}
		}

		private void GetDeviceList(long requestId)
		{
			var arg = new
			{
				deviceList = new[] {
					new {
						description = "GR-SAKURA",
						deviceId = 1,
						deviceType = "GrSakura",
						ip_addr = "192.168.2.124",
						nickname = "GR-SAKURA",
						protocol = "arduino",
						status = true
					},
					new { 
						description = "Controller",
						deviceId = 2,
						deviceType = "Controller",
						ip_addr = "192.168.2.124",
						nickname = "Controller",
						protocol = "echonetlite",
						status = true
					},
					new { 
						description = "HomeAirConditioner",
						deviceId = 4,
						deviceType = "HomeAirConditioner",
						ip_addr = "192.168.2.201",
						nickname = "HomeAirConditioner",
						protocol = "echonetlite",
						status = true
					},
					new { 
						description = "GeneralLighting",
						deviceId = 5,
						deviceType = "GeneralLighting",
						ip_addr = "192.168.2.201",
						nickname = "GeneralLighting",
						protocol = "echonetlite",
						status = true
					},
					new { 
						description = "ElectricallyOperatedShade",
						deviceId = 6,
						deviceType = "ElectricallyOperatedShade",
						ip_addr = "192.168.2.201",
						nickname = "ElectricallyOperatedShade",
						protocol = "echonetlite",
						status = true
					},
					new { 
						description = "TemperatureSensor",
						deviceId = 7,
						deviceType = "TemperatureSensor",
						ip_addr = "192.168.2.201",
						nickname = "TemperatureSensor",
						protocol = "echonetlite",
						status = false
					},
					new { 
						description = "ElectricLock",
						deviceId = 8,
						deviceType = "ElectricLock",
						ip_addr = "192.168.2.201",
						nickname = "ElectricLock",
						protocol = "echonetlite",
						status = false
					}
				}
			};

			sendCallback(String.Format("[50,{0},{1},{2},{3}]",
				requestId, "{}", "[]", DynamicJson.Serialize(arg)));
		}

		byte OperationStatus = 0x30;

		private void KadecotSet(long requestId, int deviceId, dynamic argumentsKw)
		{
			string arg = DynamicJson.Serialize(DeviceObject.KadecotSet(deviceId, argumentsKw));

			sendCallback(String.Format("[50,{0},{{\"deviceId\":{1}}},{2},{3}]",
				requestId, deviceId, "[]", arg));
		}

		private void KadecotGet(long requestId, int deviceId, dynamic argumentsKw)
		{
			var result = new
			{
				propertyName = "OperationStatus",
				propertyValue = new byte[] { OperationStatus }
			};
			string arg;

			switch (deviceId) {
			case 2:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
				arg = DynamicJson.Serialize(result);
				break;
			default:
				return;
			}
			//	arg = DeviceObject.SetDeviceObject(deviceId, argumentsKw);

			sendCallback(String.Format("[50,{0},{{\"deviceId\":{1}}},{2},{3}]",
				requestId, deviceId, "[]", arg));
		}
		/// <summary>
		/// 機器情報を返します
		/// </summary>
		/// <param name="requestId"></param>
		/// <param name="deviceId"></param>
		private void GetDeviceInfo(long requestId, int deviceId)
		{
			string arg;

			switch (deviceId) {
			case 2:
				Debug.WriteLine("case 2");
				arg = DynamicJson.Serialize(new
				{
					x1 = 0x05,
					x2 = 0xFF,
					x3 = 0x01,
					enodid = 0x01,
					properties = new[] {
						new { epc = 0x80, flag = "RULE_SET,RULE_GET,ANNOUNCE" },
						new { epc = 0x81, flag = "RULE_SET,RULE_GET,ANNOUNCE" },
						new { epc = 0x82, flag = "RULE_SET,RULE_GET,ANNOUNCE" },
						new { epc = 0x97, flag = "RULE_SET,RULE_GET,ANNOUNCE" },
						new { epc = 0x88, flag = "RULE_SET,RULE_GET,ANNOUNCE" },
						new { epc = 0x98, flag = "RULE_SET,RULE_GET,ANNOUNCE" },
						new { epc = 0x8A, flag = "RULE_SET,RULE_GET,ANNOUNCE" },
						new { epc = 0x9D, flag = "RULE_SET,RULE_GET,ANNOUNCE" },
						new { epc = 0x9E, flag = "RULE_SET,RULE_GET,ANNOUNCE" },
						new { epc = 0x9F, flag = "RULE_SET,RULE_GET,ANNOUNCE" }
					}
				});
				break;
			case 4:
				Debug.WriteLine("case 4");
				arg = DynamicJson.Serialize(
				new
				{
					x1 = 0x01,
					x2 = 0x30,
					x3 = 0x01,
					enodid = 0x03,
					properties = new[] {
						new { epc = 0x80, flag = "RULE_SET,RULE_GET,ANNOUNCE" },
						new { epc = 0xB0, flag = "RULE_SET,RULE_GET,ANNOUNCE" },
						new { epc = 0x81, flag = "RULE_SET,RULE_GET,ANNOUNCE" },
						new { epc = 0x82, flag = "RULE_GET" },
						new { epc = 0xB3, flag = "RULE_SET,RULE_GET" },
						new { epc = 0x88, flag = "RULE_GET,ANNOUNCE" },
						new { epc = 0x8A, flag = "RULE_GET" },
						new { epc = 0x9D, flag = "RULE_GET" },
						new { epc = 0x9E, flag = "RULE_GET" },
						new { epc = 0x9F, flag = "RULE_GET" }
					}
				});
				break;
			case 5:
				Debug.WriteLine("case 5");
				arg = DynamicJson.Serialize(new
				{
					x1 = 0x02,
					x2 = 0x90,
					x3 = 0x01,
					enodid = 0x03,
					properties = new[] {
						new { epc = 0x80, flag = "RULE_SET,RULE_GET,ANNOUNCE" },
						new { epc = 0x81, flag = "RULE_SET,RULE_GET,ANNOUNCE" },
						new { epc = 0x82, flag = "RULE_GET" },
						new { epc = 0xB6, flag = "RULE_SET,RULE_GET" },
						new { epc = 0x88, flag = "RULE_GET,ANNOUNCE" },
						new { epc = 0x8A, flag = "RULE_GET" },
						new { epc = 0x9D, flag = "RULE_GET" },
						new { epc = 0x9E, flag = "RULE_GET" },
						new { epc = 0x9F, flag = "RULE_GET" }
					}
				});
				break;
			case 6:
				Debug.WriteLine("case 6");
				arg = DynamicJson.Serialize(new
				{
					x1 = 0x02,
					x2 = 0x60,
					x3 = 0x01,
					enodid = 0x03,
					properties = new[] {
						new { epc = 0x80, flag = "RULE_GET,ANNOUNCE" },
						new { epc = 0xE0, flag = "RULE_SET,RULE_GET,ANNOUNCE" },
						new { epc = 0x81, flag = "RULE_SET,RULE_GET,ANNOUNCE" },
						new { epc = 0xE1, flag = "RULE_SET,RULE_GET" },
						new { epc = 0x82, flag = "RULE_GET" },
						new { epc = 0x88, flag = "RULE_GET,ANNOUNCE" },
						new { epc = 0x8A, flag = "RULE_GET" },
						new { epc = 0x9D, flag = "RULE_GET" },
						new { epc = 0x9E, flag = "RULE_GET" },
						new { epc = 0x9F, flag = "RULE_GET" }
					}
				});
				break;
			case 7:
				Debug.WriteLine("case 7");
				arg = DynamicJson.Serialize(new
				{
					x1 = 0x00,
					x2 = 0x11,
					x3 = 0x01,
					enodid = 0x03,
					properties = new[] {
						new { epc = 0x80, flag = "RULE_GET,ANNOUNCE" },
						new { epc = 0xE0, flag = "RULE_GET" },
						new { epc = 0x81, flag = "RULE_SET,RULE_GET,ANNOUNCE" },
						new { epc = 0x82, flag = "RULE_GET" },
						new { epc = 0x88, flag = "RULE_GET,ANNOUNCE" },
						new { epc = 0x8A, flag = "RULE_GET" },
						new { epc = 0x9D, flag = "RULE_GET" },
						new { epc = 0x9E, flag = "RULE_GET" },
						new { epc = 0x9F, flag = "RULE_GET" }
					}
				});
				break;
			case 8:
				Debug.WriteLine("case 8");
				arg = DynamicJson.Serialize(new
				{
					x1 = 0x02,
					x2 = 0x6F,
					x3 = 0x01,
					enodid = 0x03,
					properties = new[] {
						new { epc = 0x80, flag = "RULE_GET,ANNOUNCE" },
						new { epc = 0xE0, flag = "RULE_SET,RULE_GET,ANNOUNCE" },
						new { epc = 0x81, flag = "RULE_SET,RULE_GET,ANNOUNCE" },
						new { epc = 0x82, flag = "RULE_GET" },
						new { epc = 0xE5, flag = "ANNOUNCE" },
						new { epc = 0x88, flag = "RULE_GET,ANNOUNCE" },
						new { epc = 0x8A, flag = "RULE_GET" },
						new { epc = 0x9D, flag = "RULE_GET" },
						new { epc = 0x9E, flag = "RULE_GET" },
						new { epc = 0x9F, flag = "RULE_GET" }
					}
				});
				break;
			default:
				Debug.WriteLine("default");
				return;
			}

			sendCallback(String.Format("[50,{0},{{\"deviceId\":{1}}},{2},{3}]",
				requestId, deviceId, "[]", arg));
		}

		private void EsvGet(long requestId, int deviceId, dynamic argumentsKw)
		{
			object arg = DeviceObject.GetDeviceObject(deviceId, argumentsKw);

			if (arg == null)
				return;

			sendCallback(String.Format("[50,{0},{{\"deviceId\":{1}}},{2},{3}]",
				requestId, deviceId, "[]", DynamicJson.Serialize(arg)));

		}
		/// <summary>
		/// jp.toppers.ecnl.procedure.setが呼び出された時実行します
		/// </summary>
		/// <param name="requestId"></param>
		/// <param name="deviceId"></param>
		/// <param name="argumentsKw"></param>
		private void EsvSet(long requestId, int deviceId, dynamic argumentsKw)
		{

			object arg = DeviceObject.SetDeviceObject(deviceId, argumentsKw);
			if (arg == null)
				return;

			sendCallback(String.Format("[50,{0},{{\"deviceId\":{1}}},{2},{3}]",
				requestId, deviceId, "[]", DynamicJson.Serialize(arg)));
		}

		private void EsvSetGet(long requestId, int deviceId, dynamic argumentsKw)
		{
			object arg = DeviceObject.SetDeviceObject(deviceId, argumentsKw);
			if (arg == null)
				return;

			sendCallback(String.Format("[50,{0},{1},{2},{3}]",
				requestId, "{}", "[]", DynamicJson.Serialize(arg)));
		}

		private void PinMode(long requestId, dynamic argumentsKw)
		{
			dynamic argument = DynamicJson.Parse(((DynamicJson)argumentsKw).ToString());
			Pin.PinMode[(int)argument.pin] = (string)argument.mode;
			sendCallback(String.Format("[50,{0},{1},{2},{3}]",
			requestId, "{\"deviceId\":1}", "[]", "{}"));
		}

		private void DigitalWrite(long requestId, dynamic argumentsKw)
		{
			dynamic argument = DynamicJson.Parse(((DynamicJson)argumentsKw).ToString());

			Pin.Digital[(int)argument.pin] = (string)argument.value;
			var arg = new { pin = argument.pin, value = argument.value };

			sendCallback(String.Format("[50,{0},{1},{2},{3}]",
				requestId, "{\"deviceId\":1}", "[]", "{}"));
		}

		private void DigitalRead(long requestId, dynamic argumentsKw)
		{
			dynamic argument = DynamicJson.Parse(((DynamicJson)argumentsKw).ToString());
			var arg = new { pin = argument.pin, value = Pin.Digital[(int)argument.pin] };
			sendCallback(String.Format("[50,{0},{1},{2},{3}]",
				requestId, "{\"deviceId\":1}", "[]", DynamicJson.Serialize(arg)));
		}

		private void AnalogRead(long requestId, dynamic argumentsKw)
		{
			dynamic argument = DynamicJson.Parse(((DynamicJson)argumentsKw).ToString());

			var arg = new { pin = argument.pin, value = Pin.analogRead[(int)argument.pin] };
			sendCallback(String.Format("[50,{0},{1},{2},{3}]",
				requestId, "{\"deviceId\":1}", "[]", DynamicJson.Serialize(arg)));
		}

		private void AnalogWrite(long requestId, dynamic argumentsKw)
		{
			dynamic argument = DynamicJson.Parse(((DynamicJson)argumentsKw).ToString());
			Pin.AnalogWrite[(int)argument.pin] = int.Parse(((double)argument.value).ToString());
			sendCallback(String.Format("[50,{0},{1},{2},{3}]",
				requestId, "{\"deviceId\":1}", "[]", "{}"));
		}

		private void SendWebSocket(string jsonMsg)
		{
			ArraySegment<byte> buf = new ArraySegment<byte>(
				System.Text.Encoding.ASCII.GetBytes(jsonMsg));

			webSocket.SendAsync(buf, WebSocketMessageType.Text,
				true, CancellationToken.None);
		}
	}

	public class Device
	{
		public Device(int esv, int sender, string seoj, string deoj, object[] properties)
		{
			this.esv = esv;
			this.sender = sender;
			this.seoj = seoj;
			this.deoj = deoj;
			this.properties = new Properties[properties.Length];

			for (int i = 0; i < properties.Length; i++) {
				this.properties[i] = new Properties(properties[i]);
			}
		}

		public long esv;
		public int sender;
		public string seoj;
		public string deoj;
		public Properties[] properties;
		public object GetDevice()
		{
			object[] prop = new object[this.properties.Length];
			for (int i = 0; i < prop.Length; i++) {
				prop[i] = new { epc = this.properties[i].epc, edt = this.properties[i].edt };
			}

			object obj = new
			{
				esv = this.esv,
				sender = this.sender,
				seoj = this.seoj,
				deoj = this.deoj,
				properties = prop,
			};
			return obj;
		}

		private object[] GetProperties(int[] epc)
		{
			List<object> properties = new List<object>();
			bool flag = false;
			for (int i = 0; i < this.properties.Length; i++) {
				if (epc.Contains(this.properties[i].epc)) {
					properties.Add(new { epc = this.properties[i].epc, edt = this.properties[i].edt });
					flag = true;
				};
			}
			DynamicJson.Serialize(properties.ToArray());
			if (flag) return properties.ToArray();
			else return null;
		}

		public object GetDevice(int[] epc)
		{
			return new
			{
				esv = this.esv,
				sender = this.sender,
				seoj = this.seoj,
				deoj = this.deoj,
				properties = GetProperties(epc),
			};
		}

		public object SetProperties(Properties properties)
		{
			for (int i = 0; i < this.properties.Length; i++) {
				if (this.properties[i].epc == properties.epc) {
					this.properties[i].edt = properties.edt;
				}
			}
			return GetDevice();
		}

		public object SetProperties(Properties[] properties)
		{
			for (int i = 0; i < properties.Length; i++) {
				SetProperties(properties[i]);
			}
			return GetDevice();
		}

		public class Properties
		{
			public int epc;
			public string edt;
			public Properties(object properties)
			{
				dynamic prop = (dynamic)properties;
				this.epc = (int)prop.epc;
				this.edt = (string)prop.edt;
			}
			public Properties(int epc, string edt)
			{
				this.epc = epc;
				this.edt = edt;
			}
		}
	}

	static class Pin
	{
		static Pin()
		{
			Digital[0] = "HIGH";
			Digital[1] = "HIGH";
			Digital[2] = "HIGH";
			Digital[3] = "HIGH";
			Digital[4] = "HIGH";
			Digital[5] = "LOW";
			Digital[6] = "HIGH";
			Digital[7] = "HIGH";
			Digital[8] = "HIGH";
			Digital[9] = "HIGH";
			Digital[10] = "HIGH";
			Digital[11] = "HIGH";
			Digital[12] = "HIGH";
			Digital[13] = "HIGH";

			analogRead[14] = 100;
			analogRead[15] = 200;
			analogRead[16] = 300;
			analogRead[17] = 400;
			analogRead[18] = 500;
			analogRead[19] = 600;
		}
		public static Dictionary<int, string> Digital = new Dictionary<int, string>();
		public static Dictionary<int, string> PinMode = new Dictionary<int, string>();
		public static Dictionary<int, int> AnalogWrite = new Dictionary<int, int>();
		public static Dictionary<int, int> analogRead = new Dictionary<int, int>();
	}

	static class DeviceObject
	{
		public static object KadecotSet(int index, dynamic argumentsKw)
		{
			string propertyName = (string)argumentsKw.propertyName;
			object propertyValue = argumentsKw.propertyValue;
			string str = propertyValue.ToString();
			str = str.Replace("[", "");
			str = str.Replace("]", "");
			int i = int.Parse(str);
			int epc;

			if (propertyName == "OperationStatus") {
				epc = 0x80;
			}
			else {
				epc = Convert.ToInt32(propertyName, 16);
			}
			string edt = i.ToString("X2");

			Device device = GetDevice(index);
			Device.Properties prop = new Device.Properties(epc, edt);
			device.SetProperties(prop);

			Device set = SetDevice(index, device);
			return set.GetDevice();
		}
		public static Device GetDevice(int index)
		{
			switch (index) {
			case 2:
				return D2;
			case 4:
				return D4;
			case 5:
				return D5;
			case 6:
				return D6;
			case 7:
				return D7;
			case 8:
				return D8;
			default:
				return null;
			}
		}
		public static Device SetDevice(int index, Device device)
		{
			switch (index) {
			case 2:
				return D2 = device;
			case 4:
				return D4 = device;
			case 5:
				return D5 = device;
			case 6:
				return D6 = device;
			case 7:
				return D7 = device;
			case 8:
				return D8 = device;
			default:
				return null;
			}
		}

		public static object GetDeviceObject(int index, dynamic argumentsKw)
		{
			dynamic[] properties = (dynamic[])argumentsKw.properties;
			int[] epc;
			Device device = GetDevice(index);
			if (device == null) {
				return null;
			}
			else {
				epc = new int[properties.Length];
				for (int i = 0; i < epc.Length; i++) {
					epc[i] = (int)properties[i].epc;
				}
				return device.GetDevice(epc);
			}
		}

		public static object SetDeviceObject(int index, dynamic argumentsKw)
		{
			dynamic[] properties = (dynamic[])argumentsKw.properties;
			Device device = GetDevice(index);
			Device.Properties prop;
			for (int i = 0; i < properties.Length; i++) {
				prop = new Device.Properties((int)properties[i].epc, (string)properties[i].edt);
				device.SetProperties(prop);
			}

			Device set = SetDevice(index, device);
			return set.GetDevice();
		}

		public static Device D2 = new Device(0x72, 1, "05FF01", "0EF001", new[] {
			new {epc = 0xB6, edt = "41"},
			new {epc = 0x9F, edt = "0B808182888A97989D9E9FB6"},
			new {epc = 0x9E, edt = "0580819798B6"},
			new {epc = 0x9D, edt = "03808188"},
			new {epc = 0x98, edt = "20000000"},
			new {epc = 0x97, edt = "0000"},
			new {epc = 0x8A, edt = "0000B3"},
			new {epc = 0x88, edt = "41"},
			new {epc = 0x82, edt = "00004300"},
			new {epc = 0x81, edt = "00"},
			new {epc = 0x80, edt = "30"}
		});

		public static Device D4 = new Device(0x72, 3, "013001", "0EF001", new[] {
			new {epc = 0xB6, edt = "41"},
			new {epc = 0x9F, edt = "0B808182888A97989D9E9FB6"},
			new {epc = 0x9E, edt = "0580819798B6"},
			new {epc = 0x9D, edt = "03808188"},
			new {epc = 0x98, edt = "07DF0000"},
			new {epc = 0x97, edt = "0000"},
			new {epc = 0x8A, edt = "0000B3"},
			new {epc = 0x88, edt = "41"},
			new {epc = 0x82, edt = "00004300"},
			new {epc = 0x81, edt = "00"},
			new {epc = 0x80, edt = "30"}
		});

		public static Device D5 = new Device(0x72, 3, "029001", "0EF001", new[] {
			new {epc = 0xB6, edt = "41"},
			new {epc = 0x9F, edt = "0B808182888A97989D9E9FB6"},
			new {epc = 0x9E, edt = "0580819798B6"},
			new {epc = 0x9D, edt = "03808188"},
			new {epc = 0x98, edt = "20000000"},
			new {epc = 0x97, edt = "0000"},
			new {epc = 0x8A, edt = "0000B3"},
			new {epc = 0x88, edt = "41"},
			new {epc = 0x82, edt = "00004300"},
			new {epc = 0x81, edt = "00"},
			new {epc = 0x80, edt = "30"},
			new {epc = 0xB0, edt = "53"},
			new {epc = 0xB1, edt = "42"}
		});

		public static Device D6 = new Device(0x72, 3, "026001", "0EF001", new[] {
			new {epc = 0xB6, edt = "41"},
			new {epc = 0x9F, edt = "0B808182888A97989D9E9FB6"},
			new {epc = 0x9E, edt = "0580819798B6"},
			new {epc = 0x9D, edt = "03808188"},
			new {epc = 0x98, edt = "20000000"},
			new {epc = 0x97, edt = "0000"},
			new {epc = 0x8A, edt = "0000B3"},
			new {epc = 0x88, edt = "41"},
			new {epc = 0x82, edt = "00004300"},
			new {epc = 0x81, edt = "00"},
			new {epc = 0x80, edt = "30"}
		});

		public static Device D7 = new Device(0x72, 3, "001101", "0EF001", new[] {
			new {epc = 0xB6, edt = "41"},
			new {epc = 0x9F, edt = "0B808182888A97989D9E9FB6"},
			new {epc = 0x9E, edt = "0580819798B6"},
			new {epc = 0x9D, edt = "03808188"},
			new {epc = 0x98, edt = "07DF0000"},
			new {epc = 0x97, edt = "0000"},
			new {epc = 0x8A, edt = "0000B3"},
			new {epc = 0x88, edt = "41"},
			new {epc = 0x82, edt = "00004300"},
			new {epc = 0x81, edt = "00"},
			new {epc = 0x80, edt = "30"}
		});

		public static Device D8 = new Device(0x72, 3, "026F01", "0EF001", new[] {
			new {epc = 0xB6, edt = "41"},
			new {epc = 0x9F, edt = "0B808182888A97989D9E9FB6"},
			new {epc = 0x9E, edt = "0580819798B6"},
			new {epc = 0x9D, edt = "03808188"},
			new {epc = 0x98, edt = "20000000"},
			new {epc = 0x97, edt = "0000"},
			new {epc = 0x8A, edt = "0000B3"},
			new {epc = 0x88, edt = "41"},
			new {epc = 0x82, edt = "00004300"},
			new {epc = 0x81, edt = "00"},
			new {epc = 0x80, edt = "30"},
			new {epc = 0xB0, edt = "53"},
			new {epc = 0xB1, edt = "42"}
		});
	}
}
