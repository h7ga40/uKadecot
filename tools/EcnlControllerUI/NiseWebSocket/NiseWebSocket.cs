using System;
using System.Collections.Generic;
using System.Html;
using System.Net.WebSockets;
using System.Runtime.CompilerServices;
using System.Web;
using jQueryApi;

public class NiseEvent
{
	public string type;

	public NiseEvent(string type)
	{
		this.type = type;
	}
}

public class MessageEvent : NiseEvent
{
	public string data;

	public MessageEvent(string data)
		: base("message")
	{
		this.data = data;
	}
}

public class CloseEvent : NiseEvent
{
	public const ushort CLOSE_NORMAL = 1000;
	public const ushort CLOSE_GOING_AWAY = 1001;
	public const ushort CLOSE_PROTOCOL_ERROR = 1002;
	public const ushort CLOSE_UNSUPPORTED = 1003;
	public const ushort CLOSE_TOO_LARGE = 1004;
	public const ushort CLOSE_NO_STATUS = 1005;
	public const ushort CLOSE_ABNORMAL = 1006;

	public ushort code;
	public string reason;

	public CloseEvent(ushort code, string reason)
		: base("close")
	{
		this.code = code;
		this.reason = reason;
	}
}

public delegate void NiseHtmlEventHandler(NiseEvent @event);

public class NiseWebSocket
{
	public const ushort CLOSED = (ushort)ReadyState.Closed;
	public const ushort CLOSING = (ushort)ReadyState.Closing;
	public const ushort CONNECTING = (ushort)ReadyState.Connecting;
	public const ushort OPEN = (ushort)ReadyState.Open;

	string _Url;
	string[] _Protocols;
	ReadyState _ReadyState = ReadyState.Open;
	int _PollingTimer;

	public NiseWebSocket(string url)
	{
		int opentimer = 0;
		_ReadyState = ReadyState.Connecting;
		_Url = url.ReplaceFirst("ws://", "http://");
		opentimer = Window.SetTimeout(() =>
		{
			Window.ClearTimeout(opentimer);
			Opend();
		}, 1000);
	}

	private void Opend()
	{
		_ReadyState = ReadyState.Open;
		NiseEvent oe = new NiseEvent("open");
		if (OnOpen != null)
			OnOpen(oe);

		_PollingTimer = Window.SetInterval(() =>
		{
			jQuery.Ajax(new jQueryAjaxOptions()
			{
				Url = _Url,
				Type = "POST",
				DataType = "json",
				Data = "{\"brk_wai\":\"Polling\"}",
				Success = (respo, textStatus, request) =>
				{
					if (respo != null) {
						MessageEvent e = new MessageEvent(request.ResponseText);
						if (OnMessage != null)
							OnMessage(e);
					}
				},
				Error = (request, textStatus, error) =>
				{
				}
			});
		}, 1000);
	}

	public NiseWebSocket(string url, string protocols)
		: this(url)
	{
		_Protocols = protocols.Split(',');
	}

	public NiseWebSocket(string url, string[] protocols)
		: this(url)
	{
		_Protocols = protocols;
	}

	[IntrinsicProperty]
	public BinaryType BinaryType { get; set; }
	[IntrinsicProperty]
	public int BufferedAmount { get; private set; }
	[IntrinsicProperty]
	public string Extensions { get; private set; }
	[IntrinsicProperty]
	[ScriptName("onclose")]
	public NiseHtmlEventHandler OnClose { get; set; }
	[IntrinsicProperty]
	[ScriptName("onerror")]
	public NiseHtmlEventHandler OnError { get; set; }
	[IntrinsicProperty]
	[ScriptName("onmessage")]
	public NiseHtmlEventHandler OnMessage { get; set; }
	[IntrinsicProperty]
	[ScriptName("onopen")]
	public NiseHtmlEventHandler OnOpen { get; set; }
	[IntrinsicProperty]
	public string Protocol { get { return String.Join(",", _Protocols); } }
	[IntrinsicProperty]
	public ReadyState ReadyState { get { return _ReadyState; } }
	[IntrinsicProperty]
	public string Url { get { return _Url; } }

	public void Close()
	{
		if (_PollingTimer != 0) {
			Window.ClearInterval(_PollingTimer);
			_PollingTimer = 0;
		}

		int closetimer = 0;
		_ReadyState = ReadyState.Closing;

		closetimer = Window.SetTimeout(() =>
		{
			Window.ClearTimeout(closetimer);
			Closed(CloseEvent.CLOSE_NORMAL, "");
		}, 1000);
	}

	private void Closed(ushort code, string reason)
	{
		_ReadyState = ReadyState.Closed;
		CloseEvent oe = new CloseEvent(code, reason);
		if (OnClose != null)
			OnClose(oe);
	}

	public void Send(string data)
	{
		jQuery.Ajax(new jQueryAjaxOptions()
		{
			Url = _Url,
			Type = "POST",
			Data = data,
			DataType = "json",
			Success = (respo, textStatus, request) =>
			{
				if (respo != null) {
					MessageEvent e = new MessageEvent(request.ResponseText);
					if (OnMessage != null)
						OnMessage(e);
				}
			},
			Error = (request, textStatus, error) =>
			{
				NiseEvent e = new NiseEvent("error");
				OnError(e);
			}
		});
	}
}
