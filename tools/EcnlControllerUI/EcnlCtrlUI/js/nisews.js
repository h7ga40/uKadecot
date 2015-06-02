(function() {
	'use strict';
	var $asm = {};
	ss.initAssembly($asm, 'NiseWebSocket');
	////////////////////////////////////////////////////////////////////////////////
	// CloseEvent
	var $CloseEvent = function(code, reason) {
		this.code = 0;
		this.reason = null;
		$NiseEvent.call(this, 'close');
		this.code = code;
		this.reason = reason;
	};
	$CloseEvent.__typeName = 'CloseEvent';
	global.CloseEvent = $CloseEvent;
	////////////////////////////////////////////////////////////////////////////////
	// MessageEvent
	var $MessageEvent = function(data) {
		this.data = null;
		$NiseEvent.call(this, 'message');
		this.data = data;
	};
	$MessageEvent.__typeName = 'MessageEvent';
	global.MessageEvent = $MessageEvent;
	////////////////////////////////////////////////////////////////////////////////
	// NiseEvent
	var $NiseEvent = function(type) {
		this.type = null;
		this.type = type;
	};
	$NiseEvent.__typeName = 'NiseEvent';
	global.NiseEvent = $NiseEvent;
	////////////////////////////////////////////////////////////////////////////////
	// NiseWebSocket
	var $NiseWebSocket = function(url) {
		this.$_Url = null;
		this.$_Protocols = null;
		this.$_ReadyState = 1;
		this.$_PollingTimer = 0;
		this.binaryType = null;
		this.bufferedAmount = 0;
		this.extensions = null;
		this.onclose = null;
		this.onerror = null;
		this.onmessage = null;
		this.onopen = null;
		this.protocol = null;
		this.readyState = 0;
		this.url = null;
		var opentimer = 0;
		this.$_ReadyState = 0;
		this.$_Url = url.replace('ws://', 'http://');
		opentimer = window.setTimeout(ss.mkdel(this, function() {
			window.clearTimeout(opentimer);
			this.$opend();
		}), 1000);
	};
	$NiseWebSocket.__typeName = 'NiseWebSocket';
	$NiseWebSocket.$ctor1 = function(url, protocols) {
		$NiseWebSocket.call(this, url);
		this.$_Protocols = protocols.split(String.fromCharCode(44));
	};
	$NiseWebSocket.$ctor2 = function(url, protocols) {
		$NiseWebSocket.call(this, url);
		this.$_Protocols = protocols;
	};
	global.NiseWebSocket = $NiseWebSocket;
	ss.initClass($NiseEvent, $asm, {});
	ss.initClass($CloseEvent, $asm, {}, $NiseEvent);
	ss.initClass($MessageEvent, $asm, {}, $NiseEvent);
	ss.initClass($NiseWebSocket, $asm, {
		$opend: function() {
			this.$_ReadyState = 1;
			var oe = new $NiseEvent('open');
			if (!ss.staticEquals(this.onopen, null)) {
				this.onopen(oe);
			}
			this.$_PollingTimer = window.setInterval(ss.mkdel(this, function() {
				$.ajax({
					url: this.$_Url,
					type: 'POST',
					dataType: 'json',
					data: '{"brk_wai":"Polling"}',
					success: ss.mkdel(this, function(respo, textStatus, request) {
						if (ss.isValue(respo)) {
							var e = new $MessageEvent(request.responseText);
							if (!ss.staticEquals(this.onmessage, null)) {
								this.onmessage(e);
							}
						}
					}),
					error: function(request1, textStatus1, error) {
					}
				});
			}), 1000);
		},
		close: function() {
			if (this.$_PollingTimer !== 0) {
				window.clearInterval(this.$_PollingTimer);
				this.$_PollingTimer = 0;
			}
			var closetimer = 0;
			this.$_ReadyState = 2;
			closetimer = window.setTimeout(ss.mkdel(this, function() {
				window.clearTimeout(closetimer);
				this.$closed($CloseEvent.closE_NORMAL, '');
			}), 1000);
		},
		$closed: function(code, reason) {
			this.$_ReadyState = 3;
			var oe = new $CloseEvent(code, reason);
			if (!ss.staticEquals(this.onclose, null)) {
				this.onclose(oe);
			}
		},
		send: function(data) {
			$.ajax({ url: this.$_Url, type: 'POST', data: data, dataType: 'json', success: ss.mkdel(this, function(respo, textStatus, request) {
				if (ss.isValue(respo)) {
					var e = new $MessageEvent(request.responseText);
					if (!ss.staticEquals(this.onmessage, null)) {
						this.onmessage(e);
					}
				}
			}), error: ss.mkdel(this, function(request1, textStatus1, error) {
				var e1 = new $NiseEvent('error');
				this.onerror(e1);
			}) });
		}
	});
	$NiseWebSocket.$ctor1.prototype = $NiseWebSocket.$ctor2.prototype = $NiseWebSocket.prototype;
	(function() {
		$CloseEvent.closE_NORMAL = 1000;
		$CloseEvent.closE_GOING_AWAY = 1001;
		$CloseEvent.closE_PROTOCOL_ERROR = 1002;
		$CloseEvent.closE_UNSUPPORTED = 1003;
		$CloseEvent.closE_TOO_LARGE = 1004;
		$CloseEvent.closE_NO_STATUS = 1005;
		$CloseEvent.closE_ABNORMAL = 1006;
	})();
	(function() {
		$NiseWebSocket.CLOSED = 3;
		$NiseWebSocket.CLOSING = 2;
		$NiseWebSocket.CONNECTING = 0;
		$NiseWebSocket.OPEN = 1;
	})();
})();
