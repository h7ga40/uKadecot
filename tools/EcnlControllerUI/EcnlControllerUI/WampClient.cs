using System;
using System.Collections;
using System.Collections.Generic;
using System.Html;
using System.Net.WebSockets;
using System.Diagnostics;

namespace Kadecot
{
	public class WampClient
	{
		//delegate
		public delegate void errCallback(string err);

		//WebSocket
		private WebSocket webSocket;

		private bool challnge_flag;
		private string url;
		//		private List<string> plotocols;
		private string[] plotocols = { "wamp.2.json" };
		private bool on_open_flag;
		private int request_id;
		private errCallback errorCallback;
		private Dictionary<int, int> subscription_id_list;
		//public string origin{ get; private set;}

		//WAMP SubPlotocol
		private WampClientCore wampClientCore;

		// Use this for initialization
		//public void Awake () {
		public WampClient()
		{
			challnge_flag = false;
			url = null;
			webSocket = null;
			on_open_flag = false;
			errorCallback = null;
			request_id = 1;
			subscription_id_list = new Dictionary<int, int>();
			//origin = "http://UnityKadecotPlugin/";

			wampClientCore = new WampClientCore();
		}

		// WebSocket Connectioon
		public void connect(string serverIp)
		{
			System.Diagnostics.Debug.WriteLine("[WAMP Client] websocket connecting");
			if (on_open_flag) {
				System.Diagnostics.Debug.WriteLine("[WAMP Client] websocket already connected.");
				return;
			}
			if (!String.IsNullOrEmpty(serverIp)) {
				url = serverIp;//port : 41314
			}
			else {
				url = "ws://localhost:8080/";
			}
			webSocket = new WebSocket(url, plotocols);
			//webSocket.Origin 	 = origin;
			webSocket.OnOpen += OnOpen;
			webSocket.OnMessage += OnMessage;
			webSocket.OnClose += OnClose;
			webSocket.OnError += OnError;

			//webSocket.Connect();
		}

		public void disconnect()
		{
			System.Diagnostics.Debug.WriteLine("[WAMP Client] websocket disconnecting");
			webSocket.Close();
		}

		public void addOpenCallback(WampClientCore.WampCallback callback)
		{
			wampClientCore.openCallback = callback;
		}
		public void deleteOpenCallback()
		{
			wampClientCore.deleteOpenCallback();
		}

		public void addCloseCallback(WampClientCore.WampCallback callback)
		{
			wampClientCore.closeCallback = callback;
		}
		public void deleteCloseCallback()
		{
			wampClientCore.deleteCloseCallback();
		}

		public void addGoodbyeCallback(WampClientCore.WampCallbackArgs callback)
		{
			wampClientCore.goodbyeCallback = callback;
		}
		public void deleteGoodbyeCallback()
		{
			wampClientCore.deleteGoodbyeCallback();
		}

		public void addHeartbeatCallback(WampClientCore.WampCallbackArgs callback)
		{
			wampClientCore.heartbeatCallback = callback;
		}
		public void deleteHeartbeatCallback()
		{
			wampClientCore.deleteHeartbeatCallback();
		}

		public void addInvocationCallback(int request, WampClientCore.WampCallbackArgs callback)
		{
			wampClientCore.invocationCallback[request] = callback;
		}

		public void deleteInvocationCallback(int request)
		{
			wampClientCore.deleteInvocationCallback(request);
		}


		public string sendHello(string realm, string details, WampClientCore.WampCallbackArgs callback)
		{

			//serialize
			string data;
			data = WampSerializer.createHello(realm, details);

			//encrypt data

			//set callback
			wampClientCore.helloCallback = callback;

			//send data
			if (on_open_flag == true) {
				webSocket.Send(data);
			}
			else {
				return null;
			}

			return data;
		}

		public string sendAuthenticate(string signature, string extra, WampClientCore.WampCallbackArgs callback)
		{

			//serialize
			string data;
			data = WampSerializer.createAuthenticate(signature, extra);

			//encrypt data

			//set callback
			wampClientCore.authenticateCallback = callback;

			//send data
			if (this.on_open_flag) {
				webSocket.Send(data);
			}
			else {
				return null;
			}

			return data;
		}

		public string sendGoodbye(string details, string reason, WampClientCore.WampCallbackArgs callback)
		{

			//serialize
			string data;
			data = WampSerializer.createGoodbye(details, reason);

			//encrypt data

			//set callback
			wampClientCore.goodbyeCallback = callback;

			//send data
			if (this.on_open_flag) {
				webSocket.Send(data);
			}
			else {
				return null;
			}

			return data;
		}

		public string sendHeartbeat(int incomingSeq, int outgoingSeq, string discard)
		{

			//serialize
			string data;
			data = WampSerializer.createHeartbeat(incomingSeq, outgoingSeq, discard);

			//encrypt data

			//send data
			if (this.on_open_flag) {
				webSocket.Send(data);
			}
			else {
				return null;
			}

			return data;
		}

		public string sendError(int type, int request, string details, string error, string arguments, string argumentsKw)
		{

			//serialize
			string data;
			data = WampSerializer.createError(type, request, details, error, arguments, argumentsKw);

			//encrypt data

			//send data
			if (this.on_open_flag) {
				webSocket.Send(data);
			}
			else {
				return null;
			}

			return data;
		}


		public string sendPublish(string options, string topic, string arguments, string argumentsKw, WampClientCore.WampCallbackArgs callback)
		{

			//serialize
			string data;
			int request;
			request = this.request_id;
			this.request_id++;
			data = WampSerializer.createPublish(request, options, topic, arguments, argumentsKw);

			//encrypt data

			//set callback
			wampClientCore.publishCallback[request] = callback;

			//send data
			if (this.on_open_flag) {
				webSocket.Send(data);
			}
			else {
				return null;
			}

			return data;
		}

		public string sendSubscribe(string options, string topic, WampClientCore.WampCallbackArgs eventfunc, WampClientCore.WampCallbackArgs callback)
		{

			//serialize
			string data;
			int request;
			request = this.request_id;
			this.request_id++;
			data = WampSerializer.createSubscribe(request, options, topic);

			//encrypt data

			//set callback
			wampClientCore.subscribeCallback[request] = callback;

			//set event callback
			wampClientCore.eventCallback[request] = eventfunc;

			//send data
			if (this.on_open_flag) {
				webSocket.Send(data);
			}
			else {
				return null;
			}

			return data;
		}

		public string sendUnsubscribe(int subscription, WampClientCore.WampCallbackArgs callback)
		{

			//serialize
			string data;
			int request;
			request = this.request_id;
			this.request_id++;
			data = WampSerializer.createUnsubscribe(request, subscription);

			//encrypt data

			//set callback
			wampClientCore.unsubscribeCallback[request] = callback;

			//set Unsubscribe request id - subscription id
			this.subscription_id_list[request] = subscription;

			//send data
			if (this.on_open_flag) {
				webSocket.Send(data);
			}
			else {
				return null;
			}

			return data;
		}
		/// <summary>
		/// クライアントからメッセージを送ります
		/// </summary>
		/// <param name="options">deviceId</param>
		/// <param name="procedure"></param>
		/// <param name="arguments"></param>
		/// <param name="argumentsKw">入力するデータ</param>
		/// <param name="callback">コールバック</param>
		/// <returns>データ</returns>
		public string sendCall(string options, string procedure, string arguments, string argumentsKw, WampClientCore.WampCallbackArgs callback)
		{

			//serialize
			string data;
			int request;
			request = this.request_id;
			this.request_id++;
			data = WampSerializer.createCall(request, options, procedure, arguments, argumentsKw);

			//encrypt data

			//set callback
			wampClientCore.callCallback[request] = callback;

			//send data
			if (on_open_flag == true) {
				webSocket.Send(data);
			}
			else {
				return null;
			}

			return data;
		}

		public string sendCancel(int request, string options, WampClientCore.WampCallbackArgs callback)
		{

			//serialize
			string data;
			data = WampSerializer.createCancel(request, options);

			//encrypt data

			//set callback
			wampClientCore.cancelCallback[request] = callback;

			//send data
			if (this.on_open_flag) {
				webSocket.Send(data);
			}
			else {
				return null;
			}

			return data;
		}

		public string sendRegister(string options, string procedure, WampClientCore.WampCallbackArgs callback)
		{

			//serialize
			string data;
			int request;
			request = this.request_id;
			this.request_id++;
			data = WampSerializer.createRegister(request, options, procedure);

			//encrypt data

			//set callback
			wampClientCore.registerCallback[request] = callback;

			//send data
			if (this.on_open_flag) {
				webSocket.Send(data);
			}
			else {
				return null;
			}

			return data;
		}

		public string sendUnregister(int registration, WampClientCore.WampCallbackArgs callback)
		{

			//serialize
			string data;
			int request;
			request = this.request_id;
			this.request_id++;
			data = WampSerializer.createUnregister(request, registration);

			//encrypt data

			//set callback
			wampClientCore.unregisterCallback[request] = callback;

			//send data
			if (this.on_open_flag) {
				webSocket.Send(data);
			}
			else {
				return null;
			}

			return data;
		}

		public string sendYield(int request, string options, string arguments, string argumentsKw)
		{

			//serialize
			string data;
			data = WampSerializer.createYield(request, options, arguments, argumentsKw);

			//encrypt data

			//send data
			if (this.on_open_flag) {
				webSocket.Send(data);
			}

			return data;
		}

		public void OnOpen(Event e)
		{
			System.Diagnostics.Debug.WriteLine("[WAMP Client] websocket connected");
			on_open_flag = true;

			WampClientCore.WampCallback callback = wampClientCore.openCallback;

			if (callback != null) {
				callback();
			}
		}

		public void OnClose(Event e)
		{
			on_open_flag = false;
			System.Diagnostics.Debug.WriteLine("[WAMP Client] websocket disconnected");
			//			webSocket = null;

			WampClientCore.WampCallback callback = wampClientCore.closeCallback;

			if (callback != null) {
				callback();
			}
		}

		public void OnError(Event e)
		{
			System.Diagnostics.Debug.WriteLine("[WAMP Client] websocket error");

			dynamic/*ErrorEventArgs*/eea = (dynamic/*ErrorEventArgs*/)e;

			System.Diagnostics.Debug.WriteLine("[wamp OnError] msg :" + (string)eea.message);
			if (errorCallback != null) {
				errorCallback(eea.message);
			}
		}


		public void OnMessage(Event e)
		{
			dynamic/*MessageEventArgs*/ msg = (dynamic/*MessageEventArgs*/)e;

			//System.Diagnostics.Debug.WriteLine("[wamp perser] msg :" + msg.ToString());
			object data;
			WampClientCore.WampCallbackArgs callback = null;

			//decrypt data

			//deserialize
			try {
				if (msg.data != null) {
					data = WampDeserializer.Parse(msg.data);
				}
				else {
					data = WampDeserializer.Parse((string)msg);
				}
			}
			catch {
				data = null;
			}
			if (data == null) {
				System.Diagnostics.Debug.WriteLine("[wamp perser] Syntax error");
				return;
			}

			List<object> list = (List<object>)data;
			Int64 switchData = (Int64)list[0];
			switch (switchData) {
			case (Int64)WAMP_MSG_TYPE.WELCOME:
				// WAMP SPEC: [WELCOME, Session|id, Details|dict]
				if (this.challnge_flag) {
					this.challnge_flag = false;
					callback = wampClientCore.authenticateCallback;
					if (callback != null) {
						callback(data);
					}
					else {
						System.Diagnostics.Debug.WriteLine("[wamp perser] WELCOME Callback does not exist.");
					}
				}
				else {
					callback = wampClientCore.helloCallback;
					if (callback != null) {
						callback(data);
					}
					else {
						System.Diagnostics.Debug.WriteLine("[wamp perser] WELCOME Callback does not exist.");
					}
				}
				break;
			case (Int64)WAMP_MSG_TYPE.ABORT:
				// WAMP SPEC: [ABORT, Details|dict, Reason|uri]
				if (this.challnge_flag) {
					this.challnge_flag = false;
					callback = wampClientCore.authenticateCallback;
					if (callback != null) {
						callback(data);
					}
					else {
						System.Diagnostics.Debug.WriteLine("[wamp perser] ABORT Callback does not exist.");
					}
				}
				else {
					callback = wampClientCore.helloCallback;
					if (callback != null) {
						callback(data);
					}
					else {
						System.Diagnostics.Debug.WriteLine("[wamp perser] ABORT Callback does not exist.");
					}
				}
				break;
			case (Int64)WAMP_MSG_TYPE.CHALLENGE:
				// advanced WAMP SPEC: [CHALLENGE, AuthMethod|string, Extra|dict]
				this.challnge_flag = true;
				callback = wampClientCore.helloCallback;
				if (callback != null) {
					callback(data);
				}
				else {
					System.Diagnostics.Debug.WriteLine("[wamp perser] CHALLENGE Callback does not exist.");
				}
				break;

			case (Int64)WAMP_MSG_TYPE.GOODBYE:
				// WAMP SPEC: [GOODBYE, Details|dict, Reason|uri]
				callback = wampClientCore.goodbyeCallback;
				if (callback != null) {
					callback(data);
				}
				else {
					System.Diagnostics.Debug.WriteLine("[wamp perser] GOODBYE Callback does not exist.");
				}
				break;
			case (Int64)WAMP_MSG_TYPE.HEARTBEAT:
				// advanced WAMP SPEC: [AUTHENTICATE, Signature|string, Extra|dict]
				callback = wampClientCore.heartbeatCallback;
				if (callback != null) {
					callback(data);
				}
				else {
					System.Diagnostics.Debug.WriteLine("[wamp perser] HEARTBEAT Callback does not exist.");
				}
				break;
			case (Int64)WAMP_MSG_TYPE.ERROR: {
					// WAMP SPEC: [ERROR, REQUEST.Type|int, REQUEST.Request|id, Details|dict, Error|uri, (Arguments|list, ArgumentsKw|dict)]
					Int64 switchDataSec = (Int64)list[1];
					int request = (int)((Int64)list[2]);
					switch (switchDataSec) {
					case (Int64)WAMP_MSG_TYPE.SUBSCRIBE:
						// WAMP SPEC: [ERROR, SUBSCRIBE, SUBSCRIBE.Request|id, Details|dict, Error|uri]
						callback = wampClientCore.subscribeCallback[request];
						if (callback != null) {
							callback(data);
						}
						else {
							System.Diagnostics.Debug.WriteLine("[wamp perser] SUBSCRIBE Callback does not exist.");
						}
						break;
					case (Int64)WAMP_MSG_TYPE.UNSUBSCRIBE:
						// WAMP SPEC: [ERROR, UNSUBSCRIBE, UNSUBSCRIBE.Request|id, Details|dict, Error|uri]
						callback = wampClientCore.unsubscribeCallback[request];
						if (callback != null) {
							callback(data);
						}
						else {
							System.Diagnostics.Debug.WriteLine("[wamp perser] UNSUBSCRIBE Callback does not exist.");
						}
						break;
					case (Int64)WAMP_MSG_TYPE.PUBLISH:
						// WAMP SPEC: [ERROR, PUBLISH, PUBLISH.Request|id, Details|dict, Error|uri]
						callback = wampClientCore.publishCallback[request];
						if (callback != null) {
							callback(data);
						}
						else {
							System.Diagnostics.Debug.WriteLine("[wamp perser] PUBLISH Callback does not exist.");
						}
						break;
					case (Int64)WAMP_MSG_TYPE.INVOCATION:
						// WAMP SPEC: [ERROR, INVOCATION, INVOCATION.Request|id, Details|dict, Error|uri]
						// WAMP SPEC: [ERROR, INVOCATION, INVOCATION.Request|id, Details|dict, Error|uri, Arguments|list]
						// WAMP SPEC: [ERROR, INVOCATION, INVOCATION.Request|id, Details|dict, Error|uri, Arguments|list, ArgumentsKw|dict]
						callback = wampClientCore.invocationCallback[request];
						if (callback != null) {
							callback(data);
						}
						else {
							System.Diagnostics.Debug.WriteLine("[wamp perser] INVOCATION Callback does not exist.");
						}
						break;
					case (Int64)WAMP_MSG_TYPE.CALL:
						// WAMP SPEC: [ERROR, CALL, CALL.Request|id, Details|dict, Error|uri]
						// WAMP SPEC: [ERROR, CALL, CALL.Request|id, Details|dict, Error|uri, Arguments|list]
						// WAMP SPEC: [ERROR, CALL, CALL.Request|id, Details|dict, Error|uri, Arguments|list, ArgumentsKw|dict]
						callback = wampClientCore.callCallback[request];
						if (callback != null) {
							callback(data);
						}
						else {
							System.Diagnostics.Debug.WriteLine("[wamp perser] CALL Callback does not exist.");
						}
						break;
					case (Int64)WAMP_MSG_TYPE.REGISTER:
						// WAMP SPEC: [ERROR, REGISTER, REGISTER.Request|id, Details|dict, Error|uri]
						callback = wampClientCore.registerCallback[request];
						if (callback != null) {
							callback(data);
						}
						else {
							System.Diagnostics.Debug.WriteLine("[wamp perser] REGISTER Callback does not exist.");
						}
						break;
					case (Int64)WAMP_MSG_TYPE.UNREGISTER:
						// WAMP SPEC: [ERROR, UNREGISTER, UNREGISTER.Request|id, Details|dict, Error|uri]
						callback = wampClientCore.unregisterCallback[request];
						if (callback != null) {
							callback(data);
						}
						else {
							System.Diagnostics.Debug.WriteLine("[wamp perser] UNREGISTER Callback does not exist.");
						}
						break;
					default:
						return;
					}
					break;
				}

			case (Int64)WAMP_MSG_TYPE.PUBLISHED:
				// WAMP SPEC: [PUBLISHED, PUBLISH.Request|id, Publication|id]
				callback = wampClientCore.publishCallback[(int)((Int64)(list[1]))];
				if (callback != null) {
					callback(data);
				}
				else {
					System.Diagnostics.Debug.WriteLine("[wamp perser] Callback does not exist.");
				}
				break;
			case (Int64)WAMP_MSG_TYPE.SUBSCRIBED:
				// WAMP SPEC: [SUBSCRIBED, SUBSCRIBE.Request|id, Subscription|id]
				int reqId = (int)((Int64)(list[1]));
				int subId = (int)((Int64)(list[2]));
				callback = wampClientCore.subscribeCallback[reqId];
				if (callback != null) {
					wampClientCore.eventCallback[subId] = wampClientCore.eventCallback[reqId];
					wampClientCore.deleteEventCallback(reqId);
					callback(data);
				}
				else {
					System.Diagnostics.Debug.WriteLine("[wamp perser] Callback does not exist.");
				}
				break;
			case (Int64)WAMP_MSG_TYPE.UNSUBSCRIBED:
				// WAMP SPEC: [UNSUBSCRIBED, UNSUBSCRIBE.Request|id]
				callback = wampClientCore.unsubscribeCallback[(int)((Int64)(list[1]))];
				if (callback != null) {
					wampClientCore.deleteEventCallback(this.subscription_id_list[(int)((Int64)(list[1]))]);
					callback(data);
				}
				else {
					System.Diagnostics.Debug.WriteLine("[wamp perser] Callback does not exist.");
				}
				break;
			case (Int64)WAMP_MSG_TYPE.EVENT:
				// WAMP SPEC: [EVENT, SUBSCRIBED.Subscription|id, PUBLISHED.Publication|id, Details|dict]
				// WAMP SPEC: [EVENT, SUBSCRIBED.Subscription|id, PUBLISHED.Publication|id, Details|dict, PUBLISH.Arguments|list]
				// WAMP SPEC: [EVENT, SUBSCRIBED.Subscription|id, PUBLISHED.Publication|id, Details|dict, PUBLISH.Arguments|list, PUBLISH.ArgumentKw|dict]
				callback = wampClientCore.eventCallback[(int)((Int64)(list[1]))];
				if (callback != null) {
					callback(data);
				}
				else {
					System.Diagnostics.Debug.WriteLine("[wamp perser] Callback does not exist.");
				}
				break;
			case (Int64)WAMP_MSG_TYPE.RESULT:
				// WAMP SPEC: [RESULT, CALL.Request|id, Details|dict]
				// WAMP SPEC: [RESULT, CALL.Request|id, Details|dict, YIELD.Arguments|list]
				// WAMP SPEC: [RESULT, CALL.Request|id, Details|dict, YIELD.Arguments|list, YIELD.ArgumentsKw|dict]
				callback = wampClientCore.callCallback[(int)((Int64)(list[1]))];
				if (callback != null) {
					callback(data);
				}
				else {
					System.Diagnostics.Debug.WriteLine("[wamp perser] RESULT Callback does not exist.");
				}
				break;

			case (Int64)WAMP_MSG_TYPE.REGISTERED:
				// WAMP SPEC: [REGISTERED, REGISTER.Request|id, Registration|id]
				callback = wampClientCore.registerCallback[(int)((Int64)(list[1]))];
				if (callback != null) {
					callback(data);
				}
				else {
					System.Diagnostics.Debug.WriteLine("[wamp perser] Callback does not exist.");
				}
				break;
			case (Int64)WAMP_MSG_TYPE.UNREGISTERED:
				// WAMP SPEC: [UNREGISTERED, UNREGISTER.Request|id]
				callback = wampClientCore.unregisterCallback[(int)((Int64)(list[1]))];
				if (callback != null) {
					callback(data);
				}
				else {
					System.Diagnostics.Debug.WriteLine("[wamp perser] Callback does not exist.");
				}
				break;
			case (Int64)WAMP_MSG_TYPE.INVOCATION:
				// WAMP SPEC: [INVOCATION, Request|id, REGISTERED.Registration|id, Details|dict]
				// WAMP SPEC: [INVOCATION, Request|id, REGISTERED.Registration|id, Details|dict, CALL.Arguments|list]
				// WAMP SPEC: [INVOCATION, Request|id, REGISTERED.Registration|id, Details|dict, CALL.Arguments|list, CALL.ArgumentsKw|dict]
				callback = wampClientCore.invocationCallback[(int)((Int64)(list[1]))];
				if (callback != null) {
					callback(data);
				}
				else {
					System.Diagnostics.Debug.WriteLine("[wamp perser] Callback does not exist.");
				}
				break;
			case (Int64)WAMP_MSG_TYPE.INTERRUPT:
				// advanced WAMP SPEC: [INTERRUPT, INVOCATION.Request|id, Options|dict]
				callback = wampClientCore.interruptCallback[(int)((Int64)(list[1]))];
				if (callback != null) {
					callback(data);
				}
				else {
					System.Diagnostics.Debug.WriteLine("[wamp perser] Callback does not exist.");
				}
				break;
			default:
				System.Diagnostics.Debug.WriteLine("[wamp perser] Unknown Message Type.\n" + list.ToString());
				break;
			}
		}
		public bool isConnecting()
		{
			return on_open_flag;
		}
	}
}