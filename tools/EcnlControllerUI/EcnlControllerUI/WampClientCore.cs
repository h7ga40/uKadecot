using System.Collections;
using System.Collections.Generic;

namespace Kadecot
{
	public class WampClientCore
	{

		public delegate void WampCallback();
		public delegate void WampCallbackArgs(object args);

		public WampCallback openCallback { get; set; }
		public WampCallback closeCallback { get; set; }

		//onMessage
		public WampCallbackArgs heartbeatCallback { get; set; }
		public WampCallbackArgs helloCallback { get; set; }
		public WampCallbackArgs authenticateCallback { get; set; }
		public WampCallbackArgs goodbyeCallback { get; set; }

		public Dictionary<int, WampCallbackArgs> publishCallback { get; set; }
		public Dictionary<int, WampCallbackArgs> subscribeCallback { get; set; }
		public Dictionary<int, WampCallbackArgs> unsubscribeCallback { get; set; }
		public Dictionary<int, WampCallbackArgs> callCallback { get; set; }
		public Dictionary<int, WampCallbackArgs> cancelCallback { get; set; }
		public Dictionary<int, WampCallbackArgs> registerCallback { get; set; }
		public Dictionary<int, WampCallbackArgs> unregisterCallback { get; set; }
		public Dictionary<int, WampCallbackArgs> eventCallback { get; set; }
		public Dictionary<int, WampCallbackArgs> invocationCallback { get; set; }
		public Dictionary<int, WampCallbackArgs> interruptCallback { get; set; }

		//delete
		public void deleteOpenCallback()
		{
			openCallback = null;
		}
		public void deleteCloseCallback()
		{
			closeCallback = null;
		}
		public void deleteHeartbeatCallback()
		{
			heartbeatCallback = null;
		}
		public void deleteHelloCallback()
		{
			helloCallback = null;
		}
		public void deleteAuthenticateCallback()
		{
			authenticateCallback = null;
		}
		public void deleteGoodbyeCallback()
		{
			goodbyeCallback = null;
		}
		public void deletePublishCallback(int request)
		{
			publishCallback[request] = null;
		}
		public void deleteSubscribeCallback(int request)
		{
			subscribeCallback[request] = null;
		}
		public void deleteUnsubscribeCallback(int request)
		{
			unsubscribeCallback[request] = null;
		}
		public void deleteCallCallback(int request)
		{
			callCallback[request] = null;
		}
		public void deleteCancelCallback(int request)
		{
			cancelCallback[request] = null;
		}
		public void deleteRegisterCallback(int request)
		{
			registerCallback[request] = null;
		}
		public void deleteUnregisterCallback(int request)
		{
			unregisterCallback[request] = null;
		}
		public void deleteEventCallback(int request)
		{
			eventCallback[request] = null;
		}
		public void deleteInvocationCallback(int request)
		{
			invocationCallback[request] = null;
		}
		public void deleteInterruptCallback(int request)
		{
			interruptCallback[request] = null;
		}

		//
		public WampClientCore()
		{
			openCallback = null;
			closeCallback = null;
			heartbeatCallback = null;
			helloCallback = null;
			authenticateCallback = null;
			goodbyeCallback = null;
			publishCallback = new Dictionary<int, WampCallbackArgs>();
			subscribeCallback = new Dictionary<int, WampCallbackArgs>();
			unsubscribeCallback = new Dictionary<int, WampCallbackArgs>();
			callCallback = new Dictionary<int, WampCallbackArgs>();
			cancelCallback = new Dictionary<int, WampCallbackArgs>();
			registerCallback = new Dictionary<int, WampCallbackArgs>();
			unregisterCallback = new Dictionary<int, WampCallbackArgs>();
			eventCallback = new Dictionary<int, WampCallbackArgs>();
			invocationCallback = new Dictionary<int, WampCallbackArgs>();
			interruptCallback = new Dictionary<int, WampCallbackArgs>();
		}
	}
}