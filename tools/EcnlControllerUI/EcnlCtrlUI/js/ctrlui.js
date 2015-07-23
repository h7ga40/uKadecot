(function() {
	'use strict';
	var $asm = {};
	global.control = global.control || {};
	global.ctrlui = global.ctrlui || {};
	global.Kadecot = global.Kadecot || {};
	ss.initAssembly($asm, 'EcnlControllerUI');
	////////////////////////////////////////////////////////////////////////////////
	// CtrlUI
	var $CtrlUI = function() {
		this.$read = new (ss.makeGenericType(ss.Dictionary$2, [$ctrlui_DeviceController$DeviceInfo, $ctrlui_DeviceController$DeviceInfo]))();
		this.$m_WampClient = null;
		this.$m_DeviceController = null;
		this.$_m_CurrentNode = null;
		this.$timerId = 0;
		this.$dpi = null;
		this.$m_WampClient = new $Kadecot_WampClient();
		this.$m_WampClient.addOpenCallback(ss.mkdel(this, this.$wampClientConnected));
		this.$m_WampClient.addCloseCallback(ss.mkdel(this, this.$wampClientDisconnected));
		this.$m_WampClient.connect('ws://' + window.location.host + '/webapi');
	};
	$CtrlUI.__typeName = 'CtrlUI';
	$CtrlUI.main = function() {
		if (ss.isNullOrUndefined($CtrlUI.page)) {
			$CtrlUI.page = new $CtrlUI();
			$CtrlUI.info = new $ctrlui_Information();
		}
		$(document).on('pageinit', '#index', ss.mkdel($CtrlUI.page, $CtrlUI.page.$initClassGroups$1));
	};
	$CtrlUI.getPage = function(ele) {
		var page = ele;
		if ($(page).is('[data-role="page"]')) {
			return page;
		}
		if ($(page).parents('[data-role="page"]').length === 1) {
			return $(page).parents('[data-role="page"]').get(0);
		}
		return null;
	};
	$CtrlUI.$ajaxError = function(request, textStatus, error, url) {
		var sep = new RegExp('(\r\n|\r|\n)');
		var lines = [];
		lines.push(textStatus);
		if (ss.isInstanceOfType(error, String)) {
			lines.push(ss.cast(error, String));
		}
		else {
			lines.push(ss.cast(error.message, String));
			var stack = ss.cast(error.stack, String);
			ss.arrayAddRange(lines, stack.split(sep));
		}
		var html = $('<p>');
		for (var $t1 = 0; $t1 < lines.length; $t1++) {
			var line = lines[$t1];
			if (ss.isValue(sep.exec(line))) {
				continue;
			}
			html.append(document.createTextNode(line).wholeText);
			html.append($('<br/>'));
		}
		html.append(url + 'の読み込みに失敗しました');
		$('#popup_dialog_caption').text('通信エラー');
		$('#popup_dialog_title').text('通信エラーが発生しました');
		$('#popup_dialog_text').html(html);
		$('#popup_dialog').popup('open', { transition: 'slidedown' });
	};
	global.CtrlUI = $CtrlUI;
	////////////////////////////////////////////////////////////////////////////////
	// ElementExtensions
	var $ElementExtensions = function() {
	};
	$ElementExtensions.__typeName = 'ElementExtensions';
	global.ElementExtensions = $ElementExtensions;
	////////////////////////////////////////////////////////////////////////////////
	// jQueryMobile
	var $jQueryMobile = function() {
	};
	$jQueryMobile.__typeName = 'jQueryMobile';
	global.jQueryMobile = $jQueryMobile;
	////////////////////////////////////////////////////////////////////////////////
	// LoadingOption
	var $LoadingOption = function() {
		this.text = null;
		this.textVisible = false;
	};
	$LoadingOption.__typeName = 'LoadingOption';
	$LoadingOption.$ctor1 = function(text, textVisible) {
		this.text = null;
		this.textVisible = false;
		this.text = text;
		this.textVisible = textVisible;
	};
	global.LoadingOption = $LoadingOption;
	////////////////////////////////////////////////////////////////////////////////
	// control.KeyPress
	var $control_KeyPress = function() {
	};
	$control_KeyPress.__typeName = 'control.KeyPress';
	global.control.KeyPress = $control_KeyPress;
	////////////////////////////////////////////////////////////////////////////////
	// control.UI
	var $control_UI = function() {
	};
	$control_UI.__typeName = 'control.UI';
	$control_UI.getDeviceInfo = function(dc, dev, device) {
		var di = new $ctrlui_DeviceController$DeviceInfo.$ctor1(dc, dev);
		di.set_data(device);
		if (ss.isValue(dc)) {
			for (var $t1 = 0; $t1 < dev.properties.length; $t1++) {
				var prop = { $: dev.properties[$t1] };
				var epi = Enumerable.from(dc.properties).firstOrDefault(ss.mkdel({ prop: prop }, function(p) {
					return p.propertyCode === this.prop.$.epc;
				}), ss.getDefaultValue($ctrlui_JsonPropertyInfo));
				if (ss.isNullOrUndefined(epi)) {
					continue;
				}
				di.get_properties().push(new $ctrlui_DeviceController$DevicePropertyInfo(epi, prop.$));
			}
		}
		return di;
	};
	$control_UI.getAllDevices = function(args) {
		var devs = ss.cast(ss.cast(args, Array)[4].deviceList, Array);
		var alldevs = [];
		var nodes = new (ss.makeGenericType(ss.Dictionary$2, [String, Array]))();
		for (var $t1 = 0; $t1 < devs.length; $t1++) {
			var dev = devs[$t1];
			var device = new $ctrlui_WampApiKadecotDevice(dev);
			var devices = {};
			if (nodes.tryGetValue(device.ip_addr, devices)) {
				devices.$.push(device);
			}
			else {
				devices.$ = [];
				devices.$.push(device);
				nodes.set_item(device.ip_addr, devices.$);
			}
			alldevs.push(device);
		}
		return alldevs;
	};
	$control_UI.toJSClassGroupInfoList = function(data) {
		var _classGroups = ss.cast((ss.referenceEquals(ss.getInstanceType(data), String) ? $.parseJSON(ss.cast(data, String)) : data), Array);
		var classGroups = [];
		for (var $t1 = 0; $t1 < _classGroups.length; $t1++) {
			var _item = _classGroups[$t1];
			var item = new $ctrlui_JsonClassGroupInfo(_item);
			classGroups.push(item);
		}
		return classGroups;
	};
	$control_UI.toJSClassInfoList = function(data, classGroup) {
		var _classes = ss.cast((ss.referenceEquals(ss.getInstanceType(data), String) ? $.parseJSON(ss.cast(data, String)) : data), Array);
		var classes = [];
		var eclass;
		for (var $t1 = 0; $t1 < _classes.length; $t1++) {
			var _item = _classes[$t1];
			eclass = new $ctrlui_JsonClassInfo(_item);
			eclass.classGroup = classGroup;
			classes.push(eclass);
		}
		return classes;
	};
	$control_UI.toJSPropertyInfoList = function(data) {
		var _properties = ss.cast((ss.referenceEquals(ss.getInstanceType(data), String) ? $.parseJSON(ss.cast(data, String)) : data), Array);
		var properties = [];
		for (var $t1 = 0; $t1 < _properties.length; $t1++) {
			var _item = _properties[$t1];
			var item = new $ctrlui_JsonPropertyInfo.$ctor1(_item);
			properties.push(item);
		}
		return properties;
	};
	$control_UI.disable = function(obj, flag) {
		if (flag) {
			obj.attr('disabled', 'disabled');
		}
		else {
			obj.removeAttr('disabled');
		}
		return obj;
	};
	$control_UI.toggleClass = function(obj, deleteClass, addClass) {
		obj.toggleClass(deleteClass, false);
		obj.toggleClass(addClass, true);
		return obj;
	};
	$control_UI.getDeviceBtn = function(di) {
		return $control_UI.getDeviceLi(di).children('a');
	};
	$control_UI.getDeviceLi = function(di) {
		return $('#li_device_' + di.getObjectID());
	};
	$control_UI.isRULE_SET = function(dpi) {
		return (ss.isNullOrUndefined(dpi.get_propertyInfo()) ? false : ss.contains(dpi.get_propertyInfo().access, 'RULE_SET'));
	};
	global.control.UI = $control_UI;
	////////////////////////////////////////////////////////////////////////////////
	// control.UIAnchor
	var $control_UIAnchor = function() {
		$control_UIObject.$ctor1.call(this, '<a>');
	};
	$control_UIAnchor.__typeName = 'control.UIAnchor';
	$control_UIAnchor.$ctor1 = function(text) {
		$control_UIAnchor.call(this);
		this.set_text(text);
	};
	$control_UIAnchor.$ctor2 = function(uiobject) {
		$control_UIAnchor.call(this);
		this.append(uiobject);
	};
	global.control.UIAnchor = $control_UIAnchor;
	////////////////////////////////////////////////////////////////////////////////
	// control.UIBackButton
	var $control_UIBackButton = function() {
		$control_UIButton.call(this);
		this.set_buttonIcon(2);
		this.set_title('元に戻す');
		this.set_tabIndex(-1);
		this.addClass('text_backButton');
		this.object.hide();
	};
	$control_UIBackButton.__typeName = 'control.UIBackButton';
	global.control.UIBackButton = $control_UIBackButton;
	////////////////////////////////////////////////////////////////////////////////
	// control.UIButton
	var $control_UIButton = function() {
		$control_UIObject.$ctor1.call(this, '<a>');
		this.set_button(true);
		this.set_noText(true);
		this.set_href('#');
		this.set_cornerAll(true);
	};
	$control_UIButton.__typeName = 'control.UIButton';
	global.control.UIButton = $control_UIButton;
	////////////////////////////////////////////////////////////////////////////////
	// control.UIButton.Icon
	var $control_UIButton$Icon = function() {
	};
	$control_UIButton$Icon.__typeName = 'control.UIButton$Icon';
	global.control.UIButton$Icon = $control_UIButton$Icon;
	////////////////////////////////////////////////////////////////////////////////
	// control.UICalBox
	var $control_UICalBox = function() {
		$control_UITextInput.call(this);
		this.set_dataRole('datebox');
		this.set_dataOptions(ss.mkdict(['mode', 'calbox']));
		this.set_dataClearBtn(false);
		this.on('dateboxcreate', ss.thisFix(ss.mkdel(this, this.$dateBoxCreate)));
	};
	$control_UICalBox.__typeName = 'control.UICalBox';
	global.control.UICalBox = $control_UICalBox;
	////////////////////////////////////////////////////////////////////////////////
	// control.UICalBox.UIInputDate
	var $control_UICalBox$UIInputDate = function(ev) {
		$control_UIObject.call(this);
		this.object = $(ev.target).parent('.ui-input-datebox');
	};
	$control_UICalBox$UIInputDate.__typeName = 'control.UICalBox$UIInputDate';
	global.control.UICalBox$UIInputDate = $control_UICalBox$UIInputDate;
	////////////////////////////////////////////////////////////////////////////////
	// control.UICollapsibleset
	var $control_UICollapsibleset = function() {
		$control_UIObject.$ctor1.call(this, '<div>');
		this.set_dataRole('collapsible');
	};
	$control_UICollapsibleset.__typeName = 'control.UICollapsibleset';
	$control_UICollapsibleset.$ctor2 = function(attr, h2) {
		$control_UICollapsibleset.call(this);
		this.attribute(attr);
		this.append(new $control_UIH.$ctor2(2, h2));
	};
	$control_UICollapsibleset.$ctor1 = function(h2) {
		$control_UICollapsibleset.call(this);
		this.append(new $control_UIH.$ctor2(2, h2));
	};
	global.control.UICollapsibleset = $control_UICollapsibleset;
	////////////////////////////////////////////////////////////////////////////////
	// control.UIDeviceController
	var $control_UIDeviceController = function() {
		this.deviceController = null;
	};
	$control_UIDeviceController.__typeName = 'control.UIDeviceController';
	global.control.UIDeviceController = $control_UIDeviceController;
	////////////////////////////////////////////////////////////////////////////////
	// control.UIDeviceController.UIDeviceInfo
	var $control_UIDeviceController$UIDeviceInfo = function() {
		this.page = null;
		this.di = null;
		this.$elements = null;
	};
	$control_UIDeviceController$UIDeviceInfo.__typeName = 'control.UIDeviceController$UIDeviceInfo';
	global.control.UIDeviceController$UIDeviceInfo = $control_UIDeviceController$UIDeviceInfo;
	////////////////////////////////////////////////////////////////////////////////
	// control.UIDeviceController.UIDevicePropertyInfo
	var $control_UIDeviceController$UIDevicePropertyInfo = function() {
		this.$grpbox = null;
		this.$dpi = null;
	};
	$control_UIDeviceController$UIDevicePropertyInfo.__typeName = 'control.UIDeviceController$UIDevicePropertyInfo';
	global.control.UIDeviceController$UIDevicePropertyInfo = $control_UIDeviceController$UIDevicePropertyInfo;
	////////////////////////////////////////////////////////////////////////////////
	// control.UIDiv
	var $control_UIDiv = function() {
		$control_UIObject.$ctor1.call(this, '<div>');
	};
	$control_UIDiv.__typeName = 'control.UIDiv';
	global.control.UIDiv = $control_UIDiv;
	////////////////////////////////////////////////////////////////////////////////
	// control.UIEventHandler
	var $control_UIEventHandler = function() {
	};
	$control_UIEventHandler.__typeName = 'control.UIEventHandler';
	$control_UIEventHandler.loadingShow = function(ev) {
		$.mobile.loading('show');
	};
	$control_UIEventHandler.loadingHide = function(ev) {
		$.mobile.loading('hide');
	};
	global.control.UIEventHandler = $control_UIEventHandler;
	////////////////////////////////////////////////////////////////////////////////
	// control.UIEventNames
	var $control_UIEventNames = function() {
	};
	$control_UIEventNames.__typeName = 'control.UIEventNames';
	global.control.UIEventNames = $control_UIEventNames;
	////////////////////////////////////////////////////////////////////////////////
	// control.UIFieldContain
	var $control_UIFieldContain = function() {
		this.label = null;
		this.input = null;
		$control_UIObject.$ctor1.call(this, '<div>');
		this.set_dataRole('fieldcontain');
	};
	$control_UIFieldContain.__typeName = 'control.UIFieldContain';
	$control_UIFieldContain.$ctor1 = function(uiobject) {
		$control_UIFieldContain.call(this);
		this.append(uiobject);
	};
	$control_UIFieldContain.$ctor2 = function(label, uiobject) {
		$control_UIFieldContain.call(this);
		this.label = label;
		this.input = uiobject;
		this.append(this.label);
		this.append(this.input);
	};
	global.control.UIFieldContain = $control_UIFieldContain;
	////////////////////////////////////////////////////////////////////////////////
	// control.UIFieldSet
	var $control_UIFieldSet = function() {
		this.radioList = null;
		this.$_ControlGroup = false;
		this.$_Horizontal = false;
		$control_UIObject.$ctor1.call(this, '<fieldset>');
		this.set_controlGroup(true);
	};
	$control_UIFieldSet.__typeName = 'control.UIFieldSet';
	$control_UIFieldSet.$ctor1 = function(horizontal) {
		$control_UIFieldSet.call(this);
		this.set_horizontal(horizontal);
	};
	global.control.UIFieldSet = $control_UIFieldSet;
	////////////////////////////////////////////////////////////////////////////////
	// control.UIFieldSet.RadioInput
	var $control_UIFieldSet$RadioInput = function(Label, Radio) {
		this.radio = null;
		this.label = null;
		this.radio = Radio;
		this.label = Label;
	};
	$control_UIFieldSet$RadioInput.__typeName = 'control.UIFieldSet$RadioInput';
	global.control.UIFieldSet$RadioInput = $control_UIFieldSet$RadioInput;
	////////////////////////////////////////////////////////////////////////////////
	// control.UIFlipSwitch
	var $control_UIFlipSwitch = function() {
		this.ON = null;
		this.OFF = null;
		$control_UIObject.$ctor1.call(this, '<select>');
		this.set_dataRole('flipswitch');
		//base.Append(new UIOption());
		//base.Append(new UIOption());
	};
	$control_UIFlipSwitch.__typeName = 'control.UIFlipSwitch';
	$control_UIFlipSwitch.$ctor1 = function(off, on) {
		$control_UIFlipSwitch.call(this);
		this.OFF = off;
		this.ON = on;
		this.append(this.OFF);
		this.append(this.ON);
	};
	global.control.UIFlipSwitch = $control_UIFlipSwitch;
	////////////////////////////////////////////////////////////////////////////////
	// control.UIGridA
	var $control_UIGridA = function() {
		this.blockA = new $control_UIGridA$UIBlockA();
		this.blockB = new $control_UIGridA$UIBlockB();
		$control_UIObject.$ctor1.call(this, '<div>');
		this.object.addClass('ui-grid-a');
		this.append(this.blockA);
		this.append(this.blockB);
	};
	$control_UIGridA.__typeName = 'control.UIGridA';
	global.control.UIGridA = $control_UIGridA;
	////////////////////////////////////////////////////////////////////////////////
	// control.UIGridA.UIBlockA
	var $control_UIGridA$UIBlockA = function() {
		$control_UIObject.$ctor1.call(this, '<div>');
		this.object.addClass('ui-block-a');
	};
	$control_UIGridA$UIBlockA.__typeName = 'control.UIGridA$UIBlockA';
	global.control.UIGridA$UIBlockA = $control_UIGridA$UIBlockA;
	////////////////////////////////////////////////////////////////////////////////
	// control.UIGridA.UIBlockB
	var $control_UIGridA$UIBlockB = function() {
		$control_UIObject.$ctor1.call(this, '<div>');
		this.object.addClass('ui-block-b');
	};
	$control_UIGridA$UIBlockB.__typeName = 'control.UIGridA$UIBlockB';
	global.control.UIGridA$UIBlockB = $control_UIGridA$UIBlockB;
	////////////////////////////////////////////////////////////////////////////////
	// control.UIGrpBox
	var $control_UIGrpBox = function() {
		this.$label = null;
		this.$user = true;
		this.$dpi = null;
		this.$liAside = null;
		this.getValue = null;
		this.fieldContain = null;
		this.kadecotSet = null;
		this.backButton = new $control_UIBackButton();
		$control_UIListviewItem.call(this);
		this.$liAside = new $control_UIP.$ctor2('', 'ui-li-aside');
		this.append(this.$liAside);
		this.change$1(ss.thisFix(ss.mkdel(this, this.grpBoxChange)));
	};
	$control_UIGrpBox.__typeName = 'control.UIGrpBox';
	$control_UIGrpBox.$ctor1 = function(dpi) {
		$control_UIGrpBox.call(this);
		this.$dpi = dpi;
		this.$label = new $control_UILabel.$ctor2(dpi);
		this.$dpi.prepareInputInfo();
		if (dpi.get_propertyCode() === 0) {
		}
		else if (dpi.get_propertyCode() === 151) {
			this.$setTimebox();
		}
		else if (dpi.get_propertyCode() === 152) {
			this.$setCalBox();
		}
		else if (this.$dpi.get_inputTypes().length === 0) {
			this.$setTextInput();
		}
		else if (dpi.get_propertyCode() === 128 && dpi.get_inputTypes().length === 1 && dpi.get_inputTypes()[0].mode === 1) {
			this.setFlipSwitch();
		}
		else {
			var $t1 = dpi.get_inputTypes();
			for (var $t2 = 0; $t2 < $t1.length; $t2++) {
				var pii = $t1[$t2];
				switch (pii.mode) {
					case 1: {
						{
							this.$setFieldSet(pii);
						}
						break;
					}
					case 2: {
						{
							this.setRange(pii);
						}
						break;
					}
					default: {
						{
							this.setDefault();
						}
						break;
					}
				}
			}
		}
	};
	global.control.UIGrpBox = $control_UIGrpBox;
	////////////////////////////////////////////////////////////////////////////////
	// control.UIH
	var $control_UIH = function() {
		$control_UIObject.$ctor1.call(this, '<h1>');
	};
	$control_UIH.__typeName = 'control.UIH';
	$control_UIH.$ctor1 = function(i) {
		$control_UIObject.$ctor1.call(this, '<h' + i.toString() + '>');
	};
	$control_UIH.$ctor2 = function(i, text) {
		$control_UIH.$ctor1.call(this, i);
		this.set_text(text);
	};
	global.control.UIH = $control_UIH;
	////////////////////////////////////////////////////////////////////////////////
	// control.UIImg
	var $control_UIImg = function() {
		$control_UIObject.$ctor1.call(this, '<img>');
	};
	$control_UIImg.__typeName = 'control.UIImg';
	$control_UIImg.$ctor1 = function(attr) {
		$control_UIImg.call(this);
		this.attribute(attr);
	};
	$control_UIImg.$ctor2 = function(nameValuePairs) {
		$control_UIImg.call(this);
		this.attribute(ss.mkdict(nameValuePairs));
	};
	global.control.UIImg = $control_UIImg;
	////////////////////////////////////////////////////////////////////////////////
	// control.UILabel
	var $control_UILabel = function() {
		$control_UIObject.$ctor1.call(this, '<label>');
	};
	$control_UILabel.__typeName = 'control.UILabel';
	$control_UILabel.$ctor1 = function(text) {
		$control_UILabel.call(this);
		this.set_text(text);
	};
	$control_UILabel.$ctor3 = function(text, for1) {
		$control_UILabel.$ctor1.call(this, text);
		this.set_for(for1);
	};
	$control_UILabel.$ctor4 = function(text, for1, name) {
		$control_UILabel.$ctor3.call(this, text, for1);
		this.set_name(name);
	};
	$control_UILabel.$ctor2 = function(dpi) {
		$control_UILabel.$ctor1.call(this, dpi.getText());
		this.set_title(dpi.get_propertyInfo().valueDescription);
	};
	global.control.UILabel = $control_UILabel;
	////////////////////////////////////////////////////////////////////////////////
	// control.UILi
	var $control_UILi = function() {
		$control_UIObject.$ctor1.call(this, '<li>');
	};
	$control_UILi.__typeName = 'control.UILi';
	global.control.UILi = $control_UILi;
	////////////////////////////////////////////////////////////////////////////////
	// control.UIListDivider
	var $control_UIListDivider = function() {
		$control_UIListviewItem.call(this);
		this.set_dataRole('list-divider');
	};
	$control_UIListDivider.__typeName = 'control.UIListDivider';
	$control_UIListDivider.$ctor1 = function(text) {
		$control_UIListDivider.call(this);
		this.set_text(text);
	};
	$control_UIListDivider.$ctor2 = function(uiobject) {
		$control_UIListDivider.call(this);
		this.append(uiobject);
	};
	$control_UIListDivider.$ctor3 = function(object) {
		$control_UIListDivider.call(this);
		this.append$1(object);
	};
	global.control.UIListDivider = $control_UIListDivider;
	////////////////////////////////////////////////////////////////////////////////
	// control.UIListview
	var $control_UIListview = function() {
		$control_UIObject.$ctor1.call(this, '<ul>');
		this.set_dataRole('listview');
	};
	$control_UIListview.__typeName = 'control.UIListview';
	$control_UIListview.$ctor1 = function(nameValuePairs) {
		$control_UIListview.call(this);
		this.attribute(ss.mkdict(nameValuePairs));
	};
	global.control.UIListview = $control_UIListview;
	////////////////////////////////////////////////////////////////////////////////
	// control.UIListviewItem
	var $control_UIListviewItem = function() {
		$control_UIObject.$ctor1.call(this, '<li>');
	};
	$control_UIListviewItem.__typeName = 'control.UIListviewItem';
	$control_UIListviewItem.$ctor1 = function(uiobject) {
		$control_UIListviewItem.call(this);
		this.append(uiobject);
	};
	global.control.UIListviewItem = $control_UIListviewItem;
	////////////////////////////////////////////////////////////////////////////////
	// control.UINodeList
	var $control_UINodeList = function(NodeList, listitem_click) {
		this.$info = null;
		this.$info = new $control_UIDiv();
		var identifer = '';
		var $t1 = ss.getEnumerator(NodeList);
		try {
			while ($t1.moveNext()) {
				var node = $t1.current();
				var profile = node.get_profile();
				if ($('#cl_node_' + identifer).length > 0) {
					continue;
				}
				var col = new $control_UICollapsibleset.$ctor1(node.get_data().toString());
				col.set_id('cl_node_' + identifer);
				col.set_dataInset(true);
				this.$info.append(col);
				var listview = new $control_UIListview.$ctor1(['id', 'lv_node_' + identifer]);
				col.append(listview);
				var $t2 = node.get_devices();
				for (var $t3 = 0; $t3 < $t2.length; $t3++) {
					var item = { $: $t2[$t3] };
					if (item.$.get_profile()) {
						continue;
					}
					identifer = item.$.getObjectID();
					if ($('#li_device_' + identifer).length > 0) {
						continue;
					}
					var listitem = { $: new $control_UIListviewItem() };
					listitem.$.set_id('li_device_' + identifer);
					listitem.$.click$1(ss.thisFix(ss.mkdel({ item: item }, function(ele, ev) {
						listitem_click(ele, ev, this.item.$);
					})));
					listview.append(listitem.$);
					var anchor = new $control_UIAnchor();
					anchor.set_href('#');
					anchor.set_title(ss.cast(item.$.get_data(), $ctrlui_WampApiKadecotDevice).description);
					anchor.set_text(ss.cast(item.$.get_data(), $ctrlui_WampApiKadecotDevice).nickname);
					anchor.click$1(ss.thisFix(ss.mkdel({ listitem: listitem, item: item, $this: this }, function(ele1, ev1) {
						this.$this.$listitem_Click(this.listitem.$, this.item.$);
					})));
					listitem.$.append(anchor);
				}
			}
		}
		finally {
			$t1.dispose();
		}
	};
	$control_UINodeList.__typeName = 'control.UINodeList';
	global.control.UINodeList = $control_UINodeList;
	////////////////////////////////////////////////////////////////////////////////
	// control.UIObject
	var $control_UIObject = function() {
		this.object = null;
		this.$ecnlclass = 0;
	};
	$control_UIObject.__typeName = 'control.UIObject';
	$control_UIObject.$ctor2 = function(uiobject) {
		this.object = null;
		this.$ecnlclass = 0;
		this.object = uiobject.object;
	};
	$control_UIObject.$ctor3 = function(object) {
		this.object = null;
		this.$ecnlclass = 0;
		this.object = object;
	};
	$control_UIObject.$ctor1 = function(html) {
		this.object = null;
		this.$ecnlclass = 0;
		this.object = $(html);
	};
	global.control.UIObject = $control_UIObject;
	////////////////////////////////////////////////////////////////////////////////
	// control.UIObject.EcnlClass
	var $control_UIObject$EcnlClass = function() {
	};
	$control_UIObject$EcnlClass.__typeName = 'control.UIObject$EcnlClass';
	global.control.UIObject$EcnlClass = $control_UIObject$EcnlClass;
	////////////////////////////////////////////////////////////////////////////////
	// control.UIOption
	var $control_UIOption = function() {
		$control_UIObject.$ctor1.call(this, '<option>');
	};
	$control_UIOption.__typeName = 'control.UIOption';
	$control_UIOption.$ctor2 = function(text, value) {
		$control_UIOption.call(this);
		this.set_text(text);
		this.set_value(value);
	};
	$control_UIOption.$ctor1 = function(object) {
		$control_UIObject.$ctor3.call(this, object);
	};
	global.control.UIOption = $control_UIOption;
	////////////////////////////////////////////////////////////////////////////////
	// control.UIP
	var $control_UIP = function() {
		$control_UIObject.$ctor1.call(this, '<p>');
	};
	$control_UIP.__typeName = 'control.UIP';
	$control_UIP.$ctor1 = function(text) {
		$control_UIP.call(this);
		this.set_text(text);
	};
	$control_UIP.$ctor2 = function(text, class1) {
		$control_UIP.$ctor1.call(this, text);
		this.addClass(class1);
	};
	global.control.UIP = $control_UIP;
	////////////////////////////////////////////////////////////////////////////////
	// control.UIPin
	var $control_UIPin = function() {
		this.readButton = null;
		this.uiPinMode = null;
		this.range = null;
		this.analogRead = null;
		this.flipSwitch = null;
		this.pin = 0;
		this.listDivider = null;
		$control_UILi.call(this);
		this.addClass('pin');
		this.listDivider = new $control_UIListDivider();
		this.uiPinMode = new $control_UIFieldSet.$ctor1(true);
		this.range = new $control_UIRangeInput();
		this.readButton = new $control_UIButton();
		this.readButton.set_title('取得');
		this.readButton.set_buttonIcon(1);
	};
	$control_UIPin.__typeName = 'control.UIPin';
	$control_UIPin.$ctor1 = function(pin) {
		$control_UIPin.call(this);
		this.pin = pin;
		var Div = new $control_UIDiv();
		Div.append(new $control_UIH.$ctor2(1, 'pin' + pin.toString()));
		Div.append(this.readButton);
		this.listDivider.append(Div);
		this.$setUIPinMode(pin);
		this.$setUIDigital(pin);
		this.$setUIRange(pin);
		if (pin >= 0 && pin <= 7) {
			this.flipSwitch = new $control_UIFlipSwitch.$ctor1(new $control_UIOption.$ctor2('LOW', 'LOW'), new $control_UIOption.$ctor2('HIGH', 'HIGH'));
			this.append(new $control_UIFieldContain.$ctor1(this.uiPinMode));
			this.append(new $control_UIFieldContain.$ctor1(this.flipSwitch));
			this.append(new $control_UIFieldContain.$ctor1(this.range));
		}
		else if (pin >= 8 && pin <= 13) {
			this.flipSwitch = new $control_UIFlipSwitch.$ctor1(new $control_UIOption.$ctor2('LOW', 'LOW'), new $control_UIOption.$ctor2('HIGH', 'HIGH'));
			this.append(new $control_UIFieldContain.$ctor1(this.uiPinMode));
			this.append(new $control_UIFieldContain.$ctor1(this.flipSwitch));
		}
		else if (pin >= 14 && pin <= 19) {
			this.$setAnalogRead(pin);
			this.append(new $control_UIFieldContain.$ctor1(this.analogRead));
		}
	};
	global.control.UIPin = $control_UIPin;
	////////////////////////////////////////////////////////////////////////////////
	// control.UIRadioInput
	var $control_UIRadioInput = function() {
		$control_UIObject.$ctor1.call(this, '<input>');
		this.object.attr('type', 'radio');
	};
	$control_UIRadioInput.__typeName = 'control.UIRadioInput';
	$control_UIRadioInput.$ctor2 = function(id, name) {
		$control_UIRadioInput.call(this);
		this.set_id(id);
		this.set_name(name);
	};
	$control_UIRadioInput.$ctor3 = function(id, name, value) {
		$control_UIRadioInput.$ctor2.call(this, id, name);
		this.set_value(value);
	};
	$control_UIRadioInput.$ctor1 = function(value) {
		$control_UIRadioInput.call(this);
		this.set_value(value);
	};
	global.control.UIRadioInput = $control_UIRadioInput;
	////////////////////////////////////////////////////////////////////////////////
	// control.UIRangeInput
	var $control_UIRangeInput = function() {
		this.handle = false;
		this.sliderHandle = null;
		$control_UIObject.$ctor1.call(this, '<input>');
		this.set_type('number');
		this.set_dataType('range');
		this.set_$dataHighlight(true);
		this.keypress(ss.thisFix(ss.mkdel(this, this.$sliderKeypress)));
		this.handle = true;
		this.one('slidecreate', ss.thisFix(ss.mkdel(this, this.$sliderCreate)));
	};
	$control_UIRangeInput.__typeName = 'control.UIRangeInput';
	$control_UIRangeInput.$ctor1 = function(min, max) {
		$control_UIRangeInput.call(this);
		this.set_min(min);
		this.set_max(max);
		this.set_maxlength(this.$getMaxLength());
	};
	global.control.UIRangeInput = $control_UIRangeInput;
	////////////////////////////////////////////////////////////////////////////////
	// control.UIRangeInput.UIHandle
	var $control_UIRangeInput$UIHandle = function(ev) {
		$control_UIObject.call(this);
		this.object = $(ev.target).parent().find('.ui-slider-handle');
	};
	$control_UIRangeInput$UIHandle.__typeName = 'control.UIRangeInput$UIHandle';
	global.control.UIRangeInput$UIHandle = $control_UIRangeInput$UIHandle;
	////////////////////////////////////////////////////////////////////////////////
	// control.UIRangeInput.UISliderTrack
	var $control_UIRangeInput$UISliderTrack = function(ev) {
		$control_UIObject.call(this);
		this.object = $(ev.target).parent().find('.ui-slider-track');
	};
	$control_UIRangeInput$UISliderTrack.__typeName = 'control.UIRangeInput$UISliderTrack';
	global.control.UIRangeInput$UISliderTrack = $control_UIRangeInput$UISliderTrack;
	////////////////////////////////////////////////////////////////////////////////
	// control.UITextInput
	var $control_UITextInput = function() {
		$control_UIObject.$ctor1.call(this, '<input>');
		this.set_type('text');
		this.set_dataClearBtn(true);
	};
	$control_UITextInput.__typeName = 'control.UITextInput';
	$control_UITextInput.$ctor1 = function(dic) {
		$control_UITextInput.call(this);
		this.attribute(dic);
	};
	global.control.UITextInput = $control_UITextInput;
	////////////////////////////////////////////////////////////////////////////////
	// control.UITimeBox
	var $control_UITimeBox = function() {
		$control_UITextInput.call(this);
		this.set_dataRole('datebox');
		this.set_dataOptions(ss.mkdict(['mode', 'timebox']));
		this.set_dataClearBtn(false);
		this.on('dateboxcreate', ss.thisFix(ss.mkdel(this, this.$dateBoxCreate)));
	};
	$control_UITimeBox.__typeName = 'control.UITimeBox';
	global.control.UITimeBox = $control_UITimeBox;
	////////////////////////////////////////////////////////////////////////////////
	// ctrlui.State
	var $ctrlui_$State = function() {
	};
	$ctrlui_$State.__typeName = 'ctrlui.$State';
	////////////////////////////////////////////////////////////////////////////////
	// ctrlui.TokenInfo
	var $ctrlui_$TokenInfo = function(token, type) {
		this.$token = null;
		this.$type = 0;
		this.$token = token;
		this.$type = type;
	};
	$ctrlui_$TokenInfo.__typeName = 'ctrlui.$TokenInfo';
	////////////////////////////////////////////////////////////////////////////////
	// ctrlui.TokenType
	var $ctrlui_$TokenType = function() {
	};
	$ctrlui_$TokenType.__typeName = 'ctrlui.$TokenType';
	////////////////////////////////////////////////////////////////////////////////
	// ctrlui.ValueSet
	var $ctrlui_$ValueSet = function(values, ranges, bitField, initialValue) {
		this.$values = null;
		this.$ranges = null;
		this.$bitField = null;
		this.$initialValue = null;
		this.$values = values;
		this.$ranges = ranges;
		this.$bitField = bitField;
		this.$initialValue = initialValue;
	};
	$ctrlui_$ValueSet.__typeName = 'ctrlui.$ValueSet';
	////////////////////////////////////////////////////////////////////////////////
	// ctrlui.BitField
	var $ctrlui_BitField = function(least, most, disp) {
		this.$_Least = 0;
		this.$_Most = 0;
		this.$_Disp = null;
		this.$_Values = [];
		this.$_Ranges = [];
		this.$_InitailValue = null;
		$ctrlui_Value.call(this);
		this.$_Least = least;
		this.$_Most = most;
		this.$_Disp = disp;
	};
	$ctrlui_BitField.__typeName = 'ctrlui.BitField';
	global.ctrlui.BitField = $ctrlui_BitField;
	////////////////////////////////////////////////////////////////////////////////
	// ctrlui.BitFiledsValue
	var $ctrlui_BitFiledsValue = function(bitFlds, bitFldVals, value) {
		this.$_BitFields = null;
		this.$_BitFieldValues = null;
		this.$_Value = 0;
		$ctrlui_Value.call(this);
		this.$_BitFields = bitFlds;
		this.$_BitFieldValues = bitFldVals;
		this.$_Value = value;
	};
	$ctrlui_BitFiledsValue.__typeName = 'ctrlui.BitFiledsValue';
	global.ctrlui.BitFiledsValue = $ctrlui_BitFiledsValue;
	////////////////////////////////////////////////////////////////////////////////
	// ctrlui.DeviceController
	var $ctrlui_DeviceController = function(nodeList) {
		this.$m_NodeList = [];
		$control_UIDeviceController.call(this);
		this.$m_NodeList = nodeList;
		this.deviceController = this;
	};
	$ctrlui_DeviceController.__typeName = 'ctrlui.DeviceController';
	global.ctrlui.DeviceController = $ctrlui_DeviceController;
	////////////////////////////////////////////////////////////////////////////////
	// ctrlui.DeviceController.DeviceInfo
	var $ctrlui_DeviceController$DeviceInfo = function(eci, prps) {
		this.$m_ClassInfo = null;
		this.$m_Profile = false;
		this.$m_ClassGroupCode = 0;
		this.$m_ClassCode = 0;
		this.$m_InstanceCode = 0;
		this.$m_Properties = [];
		this.$m_EObjId = 0;
		this.$2$DataField = null;
		$control_UIDeviceController$UIDeviceInfo.call(this);
		this.$m_ClassInfo = eci;
		ss.arrayAddRange(this.$m_Properties, prps);
	};
	$ctrlui_DeviceController$DeviceInfo.__typeName = 'ctrlui.DeviceController$DeviceInfo';
	$ctrlui_DeviceController$DeviceInfo.$ctor2 = function(eci, profile, x1, x2, x3, eobjid) {
		this.$m_ClassInfo = null;
		this.$m_Profile = false;
		this.$m_ClassGroupCode = 0;
		this.$m_ClassCode = 0;
		this.$m_InstanceCode = 0;
		this.$m_Properties = [];
		this.$m_EObjId = 0;
		this.$2$DataField = null;
		$control_UIDeviceController$UIDeviceInfo.call(this);
		this.$m_ClassInfo = eci;
		this.$m_Profile = profile;
		this.$m_ClassGroupCode = x1;
		this.$m_ClassCode = x2;
		this.$m_InstanceCode = x3;
		this.$m_EObjId = eobjid;
	};
	$ctrlui_DeviceController$DeviceInfo.$ctor1 = function(eci, dev) {
		$ctrlui_DeviceController$DeviceInfo.$ctor2.call(this, eci, dev.profile, dev.x1, dev.x2, dev.x3, dev.eobjid);
	};
	global.ctrlui.DeviceController$DeviceInfo = $ctrlui_DeviceController$DeviceInfo;
	////////////////////////////////////////////////////////////////////////////////
	// ctrlui.DeviceController.DevicePropertyInfo
	var $ctrlui_DeviceController$DevicePropertyInfo = function(epi, prop) {
		$ctrlui_DeviceController$DevicePropertyInfo.$ctor1.call(this, epi, prop.epc, prop.flag, epi.size);
	};
	$ctrlui_DeviceController$DevicePropertyInfo.__typeName = 'ctrlui.DeviceController$DevicePropertyInfo';
	$ctrlui_DeviceController$DevicePropertyInfo.$ctor1 = function(epi, epc, flag, size) {
		this.$m_PropertyInfo = null;
		this.$m_PropertyCode = 0;
		this.$m_Flag = 0;
		this.$m_Size = 0;
		this.$m_Data = null;
		this.$m_InputTypes = null;
		this.$2$InputDataField = null;
		this.$2$IsWaitResField = false;
		$control_UIDeviceController$UIDevicePropertyInfo.call(this);
		this.$m_PropertyInfo = epi;
		this.$m_PropertyCode = epc;
		this.$m_Flag = flag;
		this.$m_Size = size;
		//base.SetProperty(this);
	};
	global.ctrlui.DeviceController$DevicePropertyInfo = $ctrlui_DeviceController$DevicePropertyInfo;
	////////////////////////////////////////////////////////////////////////////////
	// ctrlui.DeviceController.NodeInfo
	var $ctrlui_DeviceController$NodeInfo = function(nodeId, profile) {
		this.$m_NodeId = 0;
		this.$m_Profile = null;
		this.$m_Devices = [];
		this.$1$DataField = null;
		this.$m_NodeId = nodeId;
		this.$m_Profile = profile;
	};
	$ctrlui_DeviceController$NodeInfo.__typeName = 'ctrlui.DeviceController$NodeInfo';
	global.ctrlui.DeviceController$NodeInfo = $ctrlui_DeviceController$NodeInfo;
	////////////////////////////////////////////////////////////////////////////////
	// ctrlui.ENOD_ID
	var $ctrlui_ENOD_ID = function() {
	};
	$ctrlui_ENOD_ID.__typeName = 'ctrlui.ENOD_ID';
	global.ctrlui.ENOD_ID = $ctrlui_ENOD_ID;
	////////////////////////////////////////////////////////////////////////////////
	// ctrlui.EPC_FLAG
	var $ctrlui_EPC_FLAG = function() {
	};
	$ctrlui_EPC_FLAG.__typeName = 'ctrlui.EPC_FLAG';
	global.ctrlui.EPC_FLAG = $ctrlui_EPC_FLAG;
	////////////////////////////////////////////////////////////////////////////////
	// ctrlui.Information
	var $ctrlui_Information = function() {
		this.$info = null;
	};
	$ctrlui_Information.__typeName = 'ctrlui.Information';
	global.ctrlui.Information = $ctrlui_Information;
	////////////////////////////////////////////////////////////////////////////////
	// ctrlui.InRangeValue
	var $ctrlui_InRangeValue = function(range, value) {
		this.$_Range = null;
		this.$_Value = 0;
		$ctrlui_Value.call(this);
		this.$_Range = range;
		this.$_Value = value;
	};
	$ctrlui_InRangeValue.__typeName = 'ctrlui.InRangeValue';
	global.ctrlui.InRangeValue = $ctrlui_InRangeValue;
	////////////////////////////////////////////////////////////////////////////////
	// ctrlui.JsonClassGroupInfo
	var $ctrlui_JsonClassGroupInfo = function(p) {
		this.classGroupCode = 0;
		this.description = null;
		this.define = null;
		this.classes = null;
		this.classGroupCode = ss.unbox(ss.cast(p.classGroupCode, ss.Int32));
		this.description = ss.cast(p.description, String);
		this.define = ss.cast(p.define, String);
	};
	$ctrlui_JsonClassGroupInfo.__typeName = 'ctrlui.JsonClassGroupInfo';
	global.ctrlui.JsonClassGroupInfo = $ctrlui_JsonClassGroupInfo;
	////////////////////////////////////////////////////////////////////////////////
	// ctrlui.JsonClassInfo
	var $ctrlui_JsonClassInfo = function(p) {
		this.classCode = 0;
		this.description = null;
		this.name = null;
		this.define = null;
		this.type = null;
		this.classGroup = null;
		this.properties = null;
		this.classCode = ss.unbox(ss.cast(p.classCode, ss.Int32));
		this.description = ss.cast(p.description, String);
		this.name = ss.cast(p.name, String);
		this.define = ss.cast(p.define, String);
		this.type = ss.cast(p.type, String);
		this.classGroup = ss.cast(p.classGroup, $ctrlui_JsonClassGroupInfo);
		this.properties = ss.cast(p.properties, Array);
	};
	$ctrlui_JsonClassInfo.__typeName = 'ctrlui.JsonClassInfo';
	global.ctrlui.JsonClassInfo = $ctrlui_JsonClassInfo;
	////////////////////////////////////////////////////////////////////////////////
	// ctrlui.JsonFieldInfo
	var $ctrlui_JsonFieldInfo = function() {
		this.description = null;
		this.valueDescription = null;
		this.unitDescription = null;
		this.initialValue = null;
		this.type = null;
		this.identifer = null;
		this.primitive = false;
		this.isArray = false;
		this.arrayCount = 0;
		this.size = 0;
		this.fields = null;
	};
	$ctrlui_JsonFieldInfo.__typeName = 'ctrlui.JsonFieldInfo';
	global.ctrlui.JsonFieldInfo = $ctrlui_JsonFieldInfo;
	////////////////////////////////////////////////////////////////////////////////
	// ctrlui.JsonNodeInfo
	var $ctrlui_JsonNodeInfo = function(type, identifer, attribute) {
		this.objects = null;
		$ctrlui_JsonObjectInfo.call(this, type, identifer);
		this.attribute = attribute;
		this.objects = [];
	};
	$ctrlui_JsonNodeInfo.__typeName = 'ctrlui.JsonNodeInfo';
	global.ctrlui.JsonNodeInfo = $ctrlui_JsonNodeInfo;
	////////////////////////////////////////////////////////////////////////////////
	// ctrlui.JsonObjectInfo
	var $ctrlui_JsonObjectInfo = function(type, identifer) {
		this.type = null;
		this.parent = null;
		this.identifer = null;
		this.instatceCode = 0;
		this.attribute = null;
		this.properties = null;
		this.type = type;
		this.identifer = identifer;
		this.instatceCode = 1;
		this.attribute = 'device';
		this.properties = [];
	};
	$ctrlui_JsonObjectInfo.__typeName = 'ctrlui.JsonObjectInfo';
	global.ctrlui.JsonObjectInfo = $ctrlui_JsonObjectInfo;
	////////////////////////////////////////////////////////////////////////////////
	// ctrlui.JsonPropertyInfo
	var $ctrlui_JsonPropertyInfo = function() {
		this.classInfo = null;
		this.propertyCode = 0;
		this.access = null;
		this.required = null;
		$ctrlui_JsonFieldInfo.call(this);
	};
	$ctrlui_JsonPropertyInfo.__typeName = 'ctrlui.JsonPropertyInfo';
	$ctrlui_JsonPropertyInfo.$ctor1 = function(p) {
		this.classInfo = null;
		this.propertyCode = 0;
		this.access = null;
		this.required = null;
		$ctrlui_JsonFieldInfo.call(this);
		this.classInfo = ss.cast(p.classInfo, $ctrlui_JsonObjectInfo);
		this.propertyCode = ss.unbox(ss.cast(p.propertyCode, ss.Int32));
		this.access = ss.cast(p.access, Array);
		this.required = ss.cast(p.required, Array);
		this.description = ss.cast(p.description, String);
		this.valueDescription = ss.cast(p.valueDescription, String);
		this.unitDescription = ss.cast(p.unitDescription, String);
		this.initialValue = ss.cast(p.initialValue, String);
		this.type = ss.cast(p.type, String);
		this.identifer = ss.cast(p.identifer, String);
		this.primitive = !!p.primitive;
		this.isArray = !!p.isArray;
		this.arrayCount = ss.unbox(ss.cast(p.arrayCount, ss.Int32));
		this.size = ss.unbox(ss.cast(p.size, ss.Int32));
		this.fields = ss.cast(p.fields, Array);
	};
	global.ctrlui.JsonPropertyInfo = $ctrlui_JsonPropertyInfo;
	////////////////////////////////////////////////////////////////////////////////
	// ctrlui.JsonSkeltonResult
	var $ctrlui_JsonSkeltonResult = function() {
		this.macro = null;
		this.declare = null;
		this.predefine = null;
		this.types = null;
		this.funcs = null;
		this.instance = null;
		this.define = null;
		this.config = null;
	};
	$ctrlui_JsonSkeltonResult.__typeName = 'ctrlui.JsonSkeltonResult';
	global.ctrlui.JsonSkeltonResult = $ctrlui_JsonSkeltonResult;
	////////////////////////////////////////////////////////////////////////////////
	// ctrlui.Option
	var $ctrlui_Option = function(val, disp) {
		this.$_Val = 0;
		this.$_Disp = null;
		$ctrlui_Value.call(this);
		this.$_Val = val;
		this.$_Disp = disp;
	};
	$ctrlui_Option.__typeName = 'ctrlui.Option';
	global.ctrlui.Option = $ctrlui_Option;
	////////////////////////////////////////////////////////////////////////////////
	// ctrlui.PropertyInputInfo
	var $ctrlui_PropertyInputInfo = function() {
		this.mode = 0;
	};
	$ctrlui_PropertyInputInfo.__typeName = 'ctrlui.PropertyInputInfo';
	global.ctrlui.PropertyInputInfo = $ctrlui_PropertyInputInfo;
	////////////////////////////////////////////////////////////////////////////////
	// ctrlui.PropertyInputMode
	var $ctrlui_PropertyInputMode = function() {
	};
	$ctrlui_PropertyInputMode.__typeName = 'ctrlui.PropertyInputMode';
	global.ctrlui.PropertyInputMode = $ctrlui_PropertyInputMode;
	////////////////////////////////////////////////////////////////////////////////
	// ctrlui.PropertyRangeInput
	var $ctrlui_PropertyRangeInput = function() {
		this.maximum = 0;
		this.minimum = 0;
		this.minDescrption = null;
		this.maxDescrption = null;
		$ctrlui_PropertyInputInfo.call(this);
		this.mode = 2;
	};
	$ctrlui_PropertyRangeInput.__typeName = 'ctrlui.PropertyRangeInput';
	global.ctrlui.PropertyRangeInput = $ctrlui_PropertyRangeInput;
	////////////////////////////////////////////////////////////////////////////////
	// ctrlui.PropertySelectInput
	var $ctrlui_PropertySelectInput = function() {
		this.option = new (ss.makeGenericType(ss.Dictionary$2, [ss.Int32, String]))();
		$ctrlui_PropertyInputInfo.call(this);
		this.mode = 1;
	};
	$ctrlui_PropertySelectInput.__typeName = 'ctrlui.PropertySelectInput';
	global.ctrlui.PropertySelectInput = $ctrlui_PropertySelectInput;
	////////////////////////////////////////////////////////////////////////////////
	// ctrlui.Range
	var $ctrlui_Range = function(min, max, disp) {
		this.$_Min = 0;
		this.$_Max = 0;
		this.$_Disp = null;
		this.$_MinDisp = null;
		this.$_MaxDisp = null;
		this.$_Unit = null;
		this.$_InitialValue = 0;
		$ctrlui_Value.call(this);
		this.$_Min = min;
		this.$_Max = max;
		this.$_Disp = disp;
		this.$_MinDisp = '';
		this.$_MaxDisp = '';
		this.$_Unit = '';
		this.$_InitialValue = min;
	};
	$ctrlui_Range.__typeName = 'ctrlui.Range';
	$ctrlui_Range.$ctor1 = function(min, max, disp, minDisp, maxDisp, unit) {
		this.$_Min = 0;
		this.$_Max = 0;
		this.$_Disp = null;
		this.$_MinDisp = null;
		this.$_MaxDisp = null;
		this.$_Unit = null;
		this.$_InitialValue = 0;
		$ctrlui_Value.call(this);
		this.$_Min = min;
		this.$_Max = max;
		this.$_Disp = disp;
		this.$_MinDisp = minDisp;
		this.$_MaxDisp = maxDisp;
		this.$_Unit = unit;
		this.$_InitialValue = min;
	};
	global.ctrlui.Range = $ctrlui_Range;
	////////////////////////////////////////////////////////////////////////////////
	// ctrlui.T_ECN_EOJ
	var $ctrlui_T_ECN_EOJ = function(src, offset) {
		this.$_eoj = new Array(3);
		this.$_eoj[0] = src[offset];
		this.$_eoj[1] = src[offset + 1];
		this.$_eoj[2] = src[offset + 2];
	};
	$ctrlui_T_ECN_EOJ.__typeName = 'ctrlui.T_ECN_EOJ';
	global.ctrlui.T_ECN_EOJ = $ctrlui_T_ECN_EOJ;
	////////////////////////////////////////////////////////////////////////////////
	// ctrlui.Value
	var $ctrlui_Value = function() {
	};
	$ctrlui_Value.__typeName = 'ctrlui.Value';
	global.ctrlui.Value = $ctrlui_Value;
	////////////////////////////////////////////////////////////////////////////////
	// ctrlui.ValueRange
	var $ctrlui_ValueRange = function() {
		this.$_Values = [];
		this.$_Ranges = [];
		this.$_BitFields = [];
		this.$_MemberFields = [];
		this.$_InitailValue = null;
	};
	$ctrlui_ValueRange.__typeName = 'ctrlui.ValueRange';
	$ctrlui_ValueRange.$parseValueRange = function(input, type, values, ranges, bitFeild, initialValue) {
		bitFeild.$ = null;
		initialValue.$ = null;
		var tokens = [];
		var pos = 0;
		while (pos < input.length) {
			var ms;
			var m;
			if (ss.isValue(m = (ms = new RegExp($ctrlui_$TokenInfo.$hexValue)).exec(input.substring(pos)))) {
				tokens.push(new $ctrlui_$TokenInfo(m[0], 1));
				pos += m[0].length;
			}
			else if (ss.isValue(m = (ms = new RegExp($ctrlui_$TokenInfo.$numeric)).exec(input.substring(pos)))) {
				tokens.push(new $ctrlui_$TokenInfo(m[0], 2));
				pos += m[0].length;
			}
			else {
				if (ss.isNullOrUndefined(m = (ms = new RegExp($ctrlui_$TokenInfo.$separetor)).exec(input.substring(pos)))) {
					tokens.push(new $ctrlui_$TokenInfo(input.substring(pos), 3));
					break;
				}
				var index = pos + m.index;
				if (index === pos) {
					tokens.push(new $ctrlui_$TokenInfo(m[0], 0));
					pos += 1;
				}
				else {
					tokens.push(new $ctrlui_$TokenInfo(input.substr(pos, index - pos), 3));
					tokens.push(new $ctrlui_$TokenInfo(m[0], 0));
					pos = index + 1;
				}
			}
		}
		tokens.push(new $ctrlui_$TokenInfo('', 4));
		var OK = false;
		var sentence = [];
		var state = 0;
		for (var $t1 = 0; $t1 < tokens.length; $t1++) {
			var token = tokens[$t1];
			switch (state) {
				case 0: {
					if (token.$type === 1 || token.$type === 2) {
						sentence.push(token);
						state = 1;
						continue;
					}
					else if (token.$type === 0 && token.$token === '[') {
						if (ss.isValue(bitFeild.$)) {
							break;
						}
						state = 12;
						continue;
					}
					break;
				}
				case 1: {
					if (token.$type === 0) {
						switch (token.$token) {
							case '=': {
								state = 2;
								continue;
							}
							case '～': {
								state = 4;
								continue;
							}
						}
					}
					else if (token.$type === 4) {
						var option = new $ctrlui_Option(sentence[0].$toInt64(type), '固定');
						if (ss.isValue(bitFeild.$)) {
							bitFeild.$.$addValue(option);
						}
						else {
							if (ss.isNullOrUndefined(initialValue.$)) {
								initialValue.$ = option;
							}
							values.push(option);
						}
						ss.clear(sentence);
						state = 11;
						continue;
					}
					break;
				}
				case 2: {
					if (token.$type === 3) {
						var option1 = new $ctrlui_Option(sentence[0].$toInt64(type), token.$token);
						if (ss.isValue(bitFeild.$)) {
							bitFeild.$.$addValue(option1);
						}
						else {
							if (ss.isNullOrUndefined(initialValue.$)) {
								initialValue.$ = option1;
							}
							values.push(option1);
						}
						ss.clear(sentence);
						state = 11;
						continue;
					}
					else if (token.$type === 2) {
						sentence.push(token);
						state = 3;
						continue;
					}
					break;
				}
				case 3: {
					if (token.$type === 3) {
						var option2 = new $ctrlui_Option(sentence[0].$toInt64(type), sentence[1].$token + token.$token);
						if (ss.isValue(bitFeild.$)) {
							bitFeild.$.$addValue(option2);
						}
						else {
							if (ss.isNullOrUndefined(initialValue.$)) {
								initialValue.$ = option2;
							}
							values.push(option2);
						}
						ss.clear(sentence);
						state = 11;
						continue;
					}
					else if (token.$type === 0 && token.$token === ',') {
						var option3 = new $ctrlui_Option(sentence[0].$toInt64(type), sentence[1].$token);
						if (ss.isValue(bitFeild.$)) {
							bitFeild.$.$addValue(option3);
						}
						else {
							if (ss.isNullOrUndefined(initialValue.$)) {
								initialValue.$ = option3;
							}
							values.push(option3);
						}
						ss.clear(sentence);
						state = 0;
						continue;
					}
					else if (token.$type === 4) {
						var option4 = new $ctrlui_Option(sentence[0].$toInt64(type), sentence[1].$token);
						if (ss.isValue(bitFeild.$)) {
							bitFeild.$.$addValue(option4);
						}
						else {
							if (ss.isNullOrUndefined(initialValue.$)) {
								initialValue.$ = option4;
							}
							values.push(option4);
						}
						ss.clear(sentence);
						OK = true;
					}
					break;
				}
				case 4: {
					if (token.$type === 1 || token.$type === 2) {
						sentence.push(token);
						state = 5;
						continue;
					}
					else if (token.$type === 0 && token.$token === '=') {
						var max = '0x';
						for (var i = sentence[0].$token.length - 2; i > 0; i--) {
							max += 'F';
						}
						sentence.push(new $ctrlui_$TokenInfo(max, 1));
						state = 6;
						continue;
					}
					break;
				}
				case 5: {
					if (token.$type === 0) {
						switch (token.$token) {
							case '=': {
								if (sentence.length === 2) {
									state = 6;
									continue;
								}
								break;
							}
							case '(': {
								sentence.push(new $ctrlui_$TokenInfo('', 3));
								state = 7;
								continue;
							}
							case ',': {
								var range = new $ctrlui_Range(sentence[0].$toInt64(type), sentence[1].$toInt64(type), ((sentence.length > 2) ? sentence[2].$token : ''));
								if (ss.isValue(bitFeild.$)) {
									bitFeild.$.$addRange(range);
								}
								else {
									if (ss.isNullOrUndefined(initialValue.$)) {
										initialValue.$ = range;
									}
									ranges.push(range);
								}
								ss.clear(sentence);
								state = 0;
								continue;
							}
						}
					}
					else if (token.$type === 4) {
						var range1 = new $ctrlui_Range(sentence[0].$toInt64(type), sentence[1].$toInt64(type), ((sentence.length > 2) ? sentence[2].$token : ''));
						if (ss.isValue(bitFeild.$)) {
							bitFeild.$.$addRange(range1);
						}
						else {
							if (ss.isNullOrUndefined(initialValue.$)) {
								initialValue.$ = range1;
							}
							ranges.push(range1);
						}
						ss.clear(sentence);
						OK = true;
					}
					break;
				}
				case 6: {
					if (token.$type === 3) {
						sentence.push(token);
						state = 5;
						continue;
					}
					break;
				}
				case 7: {
					if (token.$type === 2 || token.$type === 3) {
						sentence.push(token);
						state = 8;
						continue;
					}
					break;
				}
				case 8: {
					if (token.$type === 0 && token.$token === '～') {
						state = 9;
						continue;
					}
					break;
				}
				case 9: {
					if (token.$type === 2 || token.$type === 3) {
						sentence.push(token);
						state = 10;
						continue;
					}
					break;
				}
				case 10: {
					if (token.$type === 0 && token.$token === ')') {
						var range2 = new $ctrlui_Range.$ctor1(sentence[0].$toInt64(type), sentence[1].$toInt64(type), sentence[2].$token, sentence[3].$token, sentence[4].$token, ((sentence.length > 5) ? sentence[5].$token : ''));
						if (ss.isValue(bitFeild.$)) {
							bitFeild.$.$addRange(range2);
						}
						else {
							if (ss.isNullOrUndefined(initialValue.$)) {
								initialValue.$ = range2;
							}
							ranges.push(range2);
						}
						ss.clear(sentence);
						state = 11;
						continue;
					}
					else if (token.$type === 3) {
						sentence.push(token);
						state = 10;
						continue;
					}
					break;
				}
				case 12: {
					if (token.$type === 2) {
						sentence.push(token);
						state = 13;
						continue;
					}
					break;
				}
				case 13: {
					if (token.$type === 0) {
						switch (token.$token) {
							case '～': {
								state = 14;
								continue;
							}
							case ']': {
								sentence.push(sentence[0]);
								state = 15;
								continue;
							}
						}
					}
					break;
				}
				case 14: {
					if (token.$type === 2) {
						sentence.push(token);
						state = 16;
						continue;
					}
					break;
				}
				case 16: {
					if (token.$type === 0 && token.$token === ']') {
						state = 15;
						continue;
					}
					break;
				}
				case 15: {
					if (token.$type === 3) {
						sentence.push(token);
						state = 17;
						continue;
					}
					break;
				}
				case 17: {
					if (token.$type === 0 && token.$token === ':') {
						bitFeild.$ = new $ctrlui_BitField(sentence[0].$toInt32(), sentence[1].$toInt32(), sentence[2].$token);
						if (ss.isNullOrUndefined(initialValue.$)) {
							initialValue.$ = bitFeild.$;
						}
						ss.clear(sentence);
						state = 0;
						continue;
					}
					else if (token.$type === 4) {
						bitFeild.$ = new $ctrlui_BitField(sentence[0].$toInt32(), sentence[1].$toInt32(), sentence[2].$token);
						if (ss.isNullOrUndefined(initialValue.$)) {
							initialValue.$ = bitFeild.$;
						}
						ss.clear(sentence);
						OK = true;
						break;
					}
					break;
				}
				case 11: {
					if (token.$type === 4) {
						OK = true;
					}
					else if (token.$type === 0 && token.$token === ',') {
						state = 0;
						continue;
					}
					break;
				}
			}
			break;
		}
		return OK;
	};
	$ctrlui_ValueRange.parse = function(valrng, emti) {
		var valueRange = new $ctrlui_ValueRange();
		if (emti.primitive) {
			var lines = valrng.split(new RegExp('\r\n'));
			for (var $t1 = 0; $t1 < lines.length; $t1++) {
				var line = lines[$t1];
				if ((new RegExp('^<(.+)>$')).test(line)) {
					continue;
				}
				var values;
				var ranges;
				var bitField = {};
				var initialValue = {};
				var pair = {};
				if ($ctrlui_ValueRange.$_ParsedText.tryGetValue(line, pair)) {
					values = pair.$.$values;
					ranges = pair.$.$ranges;
					bitField.$ = pair.$.$bitField;
					initialValue.$ = pair.$.$initialValue;
				}
				else {
					values = [];
					ranges = [];
					$ctrlui_ValueRange.$parseValueRange(line, emti.type, values, ranges, bitField, initialValue);
					$ctrlui_ValueRange.$_ParsedText.add(line, new $ctrlui_$ValueSet(values, ranges, bitField.$, initialValue.$));
				}
				ss.arrayAddRange(valueRange.$_Values, values);
				ss.arrayAddRange(valueRange.$_Ranges, ranges);
				if (ss.isValue(bitField.$)) {
					valueRange.$_BitFields.push(bitField.$);
				}
				if (ss.isNullOrUndefined(valueRange.$_InitailValue)) {
					valueRange.$_InitailValue = initialValue.$;
				}
			}
		}
		else {
			for (var $t2 = 0; $t2 < emti.fields.length; $t2++) {
				var efi = emti.fields[$t2];
				var member = $ctrlui_ValueRange.parse(efi.valueDescription, efi);
				valueRange.$_MemberFields.push(member);
			}
		}
		return valueRange;
	};
	global.ctrlui.ValueRange = $ctrlui_ValueRange;
	////////////////////////////////////////////////////////////////////////////////
	// ctrlui.WampApiKadecotDevice
	var $ctrlui_WampApiKadecotDevice = function(doc) {
		this.description = null;
		this.deviceId = 0;
		this.deviceType = null;
		this.ip_addr = null;
		this.nickname = null;
		this.protocol = null;
		this.status = false;
		this.description = ss.cast(doc.description, String);
		this.deviceId = ss.unbox(ss.cast(doc.deviceId, ss.Int32));
		this.deviceType = ss.cast(doc.deviceType, String);
		this.ip_addr = ss.cast(doc.ip_addr, String);
		this.nickname = ss.cast(doc.nickname, String);
		this.protocol = ss.cast(doc.protocol, String);
		this.status = !!doc.status;
	};
	$ctrlui_WampApiKadecotDevice.__typeName = 'ctrlui.WampApiKadecotDevice';
	global.ctrlui.WampApiKadecotDevice = $ctrlui_WampApiKadecotDevice;
	////////////////////////////////////////////////////////////////////////////////
	// ctrlui.WampApiKadecotGet
	var $ctrlui_WampApiKadecotGet = function(deviceId, propertyName) {
		this.deviceId = 0;
		this.propertyName = null;
		this.deviceId = deviceId;
		this.propertyName = propertyName;
	};
	$ctrlui_WampApiKadecotGet.__typeName = 'ctrlui.WampApiKadecotGet';
	global.ctrlui.WampApiKadecotGet = $ctrlui_WampApiKadecotGet;
	////////////////////////////////////////////////////////////////////////////////
	// ctrlui.WampApiKadecotRes
	var $ctrlui_WampApiKadecotRes = function(item) {
		this.propertyName = null;
		this.propertyValue = null;
		this.propertyName = ss.cast(item.propertyName, String);
		if (ss.isValue(ss.getMembers(ss.getInstanceType(item), 16, 284, 'propertyValue'))) {
			this.propertyValue = ss.cast(item.propertyValue, Array);
		}
		else {
			this.propertyValue = [];
		}
	};
	$ctrlui_WampApiKadecotRes.__typeName = 'ctrlui.WampApiKadecotRes';
	global.ctrlui.WampApiKadecotRes = $ctrlui_WampApiKadecotRes;
	////////////////////////////////////////////////////////////////////////////////
	// ctrlui.WampApiKadecotSet
	var $ctrlui_WampApiKadecotSet = function(deviceId, propertyName, propertyValue) {
		this.deviceId = 0;
		this.propertyName = null;
		this.propertyValue = null;
		this.deviceId = deviceId;
		this.propertyName = propertyName;
		this.propertyValue = propertyValue;
	};
	$ctrlui_WampApiKadecotSet.__typeName = 'ctrlui.WampApiKadecotSet';
	global.ctrlui.WampApiKadecotSet = $ctrlui_WampApiKadecotSet;
	////////////////////////////////////////////////////////////////////////////////
	// ctrlui.WebApiEchonetMessage
	var $ctrlui_WebApiEchonetMessage = function(esv) {
		this.esv = 0;
		this.sender = 0;
		this.seoj = new $ctrlui_T_ECN_EOJ(new Array(3), 0);
		this.deoj = new $ctrlui_T_ECN_EOJ(new Array(3), 0);
		this.properties = [];
		this.esv = ss.unbox(ss.cast(esv.esv, ss.Int32));
		this.sender = ss.cast(esv.sender, ss.Int32);
		var sseoj = ss.cast(esv.seoj, String);
		this.seoj.set_x1(parseInt(sseoj.substr(0, 2), 16));
		this.seoj.set_x2(parseInt(sseoj.substr(2, 2), 16));
		this.seoj.set_x3(parseInt(sseoj.substr(4, 2), 16));
		var $t1 = ss.cast(esv.properties, Array);
		for (var $t2 = 0; $t2 < $t1.length; $t2++) {
			var prop = $t1[$t2];
			this.properties.push(new $ctrlui_WebApiPropertyData(prop));
		}
	};
	$ctrlui_WebApiEchonetMessage.__typeName = 'ctrlui.WebApiEchonetMessage';
	global.ctrlui.WebApiEchonetMessage = $ctrlui_WebApiEchonetMessage;
	////////////////////////////////////////////////////////////////////////////////
	// ctrlui.WebApiGet
	var $ctrlui_WebApiGet = function() {
		this.deojid = 0;
		this.properties = [];
	};
	$ctrlui_WebApiGet.__typeName = 'ctrlui.WebApiGet';
	$ctrlui_WebApiGet.$ctor1 = function(esv) {
		this.deojid = 0;
		this.properties = [];
		this.deojid = ss.cast(esv.deojid, ss.Int32);
		var $t1 = ss.cast(esv.properties, Array);
		for (var $t2 = 0; $t2 < $t1.length; $t2++) {
			var prop = $t1[$t2];
			this.properties.push(new $ctrlui_WebApiPropertyCode.$ctor1(prop, true));
		}
	};
	global.ctrlui.WebApiGet = $ctrlui_WebApiGet;
	////////////////////////////////////////////////////////////////////////////////
	// ctrlui.WebApiGrsaguraMessage
	var $ctrlui_WebApiGrsaguraMessage = function(esv) {
		this.pin = 0;
		this.value = null;
		this.pin = ss.unbox(ss.cast(esv.pin, ss.Int32));
		if (ss.referenceEquals(ss.getInstanceType(esv.value), Number)) {
			this.value = ss.unbox(ss.cast(esv.value, Number)).toString();
		}
		else {
			this.value = ss.cast(esv.value, String);
		}
	};
	$ctrlui_WebApiGrsaguraMessage.__typeName = 'ctrlui.WebApiGrsaguraMessage';
	global.ctrlui.WebApiGrsaguraMessage = $ctrlui_WebApiGrsaguraMessage;
	////////////////////////////////////////////////////////////////////////////////
	// ctrlui.WebApiObjectInfo
	var $ctrlui_WebApiObjectInfo = function(eobjid, obj, profile) {
		this.profile = false;
		this.x1 = 0;
		this.x2 = 0;
		this.x3 = 0;
		this.eobjid = 0;
		this.enodid = 0;
		this.properties = [];
		this.eobjid = eobjid;
		this.profile = profile;
		this.x1 = ss.unbox(ss.cast(obj.x1, ss.Int32));
		this.x2 = ss.unbox(ss.cast(obj.x2, ss.Int32));
		this.x3 = ss.unbox(ss.cast(obj.x3, ss.Int32));
		this.enodid = ss.unbox(ss.cast(obj.enodid, ss.Int32));
		var $t1 = ss.cast(obj.properties, Array);
		for (var $t2 = 0; $t2 < $t1.length; $t2++) {
			var prop = $t1[$t2];
			this.properties.push(new $ctrlui_WebApiPropertyInfo(prop));
		}
	};
	$ctrlui_WebApiObjectInfo.__typeName = 'ctrlui.WebApiObjectInfo';
	global.ctrlui.WebApiObjectInfo = $ctrlui_WebApiObjectInfo;
	////////////////////////////////////////////////////////////////////////////////
	// ctrlui.WebApiPropertyCode
	var $ctrlui_WebApiPropertyCode = function(epc) {
		this.epc = 0;
		this.epc = epc;
	};
	$ctrlui_WebApiPropertyCode.__typeName = 'ctrlui.WebApiPropertyCode';
	$ctrlui_WebApiPropertyCode.$ctor1 = function(prop, dummy) {
		this.epc = 0;
		this.epc = ss.unbox(ss.cast(prop.epc, ss.Int32));
	};
	global.ctrlui.WebApiPropertyCode = $ctrlui_WebApiPropertyCode;
	////////////////////////////////////////////////////////////////////////////////
	// ctrlui.WebApiPropertyData
	var $ctrlui_WebApiPropertyData = function(prop) {
		this.epc = 0;
		this.edt = null;
		var _edt;
		this.epc = ss.unbox(ss.cast(prop.epc, ss.Int32));
		var text = ss.cast(prop.edt, String);
		if (text.length > 0) {
			_edt = new Array(ss.Int32.div(text.length, 2));
			for (var i = 0, j = 0; j < text.length; j += 2) {
				_edt[i++] = parseInt(text.substr(j, 2), 16);
			}
		}
		else {
			_edt = [];
		}
		this.edt = this.$setEdt(_edt);
	};
	$ctrlui_WebApiPropertyData.__typeName = 'ctrlui.WebApiPropertyData';
	$ctrlui_WebApiPropertyData.$ctor1 = function(epc, edt) {
		this.epc = 0;
		this.edt = null;
		this.epc = epc;
		this.edt = this.$setEdt(edt);
	};
	global.ctrlui.WebApiPropertyData = $ctrlui_WebApiPropertyData;
	////////////////////////////////////////////////////////////////////////////////
	// ctrlui.WebApiPropertyInfo
	var $ctrlui_WebApiPropertyInfo = function(prop) {
		this.epc = 0;
		this.flag = 0;
		this.epc = ss.unbox(ss.cast(prop.epc, ss.Int32));
		this.flag = 0;
		var temp = ss.cast(prop.flag, String);
		if (temp.indexOf('RULE_ANNO') !== -1) {
			this.flag |= 4;
		}
		if (temp.indexOf('RULE_SET') !== -1) {
			this.flag |= 1;
		}
		if (temp.indexOf('RULE_GET') !== -1) {
			this.flag |= 2;
		}
		if (temp.indexOf('ANNOUNCE') !== -1) {
			this.flag |= 64;
		}
		if (temp.indexOf('VARIABLE') !== -1) {
			this.flag |= 128;
		}
	};
	$ctrlui_WebApiPropertyInfo.__typeName = 'ctrlui.WebApiPropertyInfo';
	global.ctrlui.WebApiPropertyInfo = $ctrlui_WebApiPropertyInfo;
	////////////////////////////////////////////////////////////////////////////////
	// ctrlui.WebApiSet
	var $ctrlui_WebApiSet = function() {
		this.deojid = 0;
		this.properties = [];
	};
	$ctrlui_WebApiSet.__typeName = 'ctrlui.WebApiSet';
	$ctrlui_WebApiSet.$ctor1 = function(esv) {
		this.deojid = 0;
		this.properties = [];
		this.deojid = ss.cast(esv.deojid, ss.Int32);
		var $t1 = ss.cast(esv.properties, Array);
		for (var $t2 = 0; $t2 < $t1.length; $t2++) {
			var prop = $t1[$t2];
			this.properties.push(new $ctrlui_WebApiPropertyData(prop));
		}
	};
	global.ctrlui.WebApiSet = $ctrlui_WebApiSet;
	////////////////////////////////////////////////////////////////////////////////
	// ctrlui.WebApiSetGet
	var $ctrlui_WebApiSetGet = function() {
		this.deojid = 0;
		this.setProperties = [];
		this.getProperties = [];
	};
	$ctrlui_WebApiSetGet.__typeName = 'ctrlui.WebApiSetGet';
	$ctrlui_WebApiSetGet.$ctor1 = function(esv) {
		this.deojid = 0;
		this.setProperties = [];
		this.getProperties = [];
		this.deojid = ss.cast(esv.deojid, ss.Int32);
		var $t1 = ss.cast(esv.properties, Array);
		for (var $t2 = 0; $t2 < $t1.length; $t2++) {
			var prop = $t1[$t2];
			this.getProperties.push(new $ctrlui_WebApiPropertyCode.$ctor1(prop, true));
		}
	};
	global.ctrlui.WebApiSetGet = $ctrlui_WebApiSetGet;
	////////////////////////////////////////////////////////////////////////////////
	// Kadecot.WAMP_MSG_TYPE
	var $Kadecot_WAMP_MSG_TYPE = function() {
	};
	$Kadecot_WAMP_MSG_TYPE.__typeName = 'Kadecot.WAMP_MSG_TYPE';
	global.Kadecot.WAMP_MSG_TYPE = $Kadecot_WAMP_MSG_TYPE;
	////////////////////////////////////////////////////////////////////////////////
	// Kadecot.WampClient
	var $Kadecot_WampClient = function() {
		this.$webSocket = null;
		this.$challnge_flag = false;
		this.$url = null;
		this.$plotocols = ['wamp.2.json'];
		this.$on_open_flag = false;
		this.$request_id = 0;
		this.$errorCallback = null;
		this.$subscription_id_list = null;
		this.$wampClientCore = null;
		this.$challnge_flag = false;
		this.$url = null;
		this.$webSocket = null;
		this.$on_open_flag = false;
		this.$errorCallback = null;
		this.$request_id = 1;
		this.$subscription_id_list = new (ss.makeGenericType(ss.Dictionary$2, [ss.Int32, ss.Int32]))();
		//origin = "http://UnityKadecotPlugin/";
		this.$wampClientCore = new $Kadecot_WampClientCore();
	};
	$Kadecot_WampClient.__typeName = 'Kadecot.WampClient';
	global.Kadecot.WampClient = $Kadecot_WampClient;
	////////////////////////////////////////////////////////////////////////////////
	// Kadecot.WampClientCore
	var $Kadecot_WampClientCore = function() {
		this.$1$openCallbackField = null;
		this.$1$closeCallbackField = null;
		this.$1$heartbeatCallbackField = null;
		this.$1$helloCallbackField = null;
		this.$1$authenticateCallbackField = null;
		this.$1$goodbyeCallbackField = null;
		this.$1$publishCallbackField = null;
		this.$1$subscribeCallbackField = null;
		this.$1$unsubscribeCallbackField = null;
		this.$1$callCallbackField = null;
		this.$1$cancelCallbackField = null;
		this.$1$registerCallbackField = null;
		this.$1$unregisterCallbackField = null;
		this.$1$eventCallbackField = null;
		this.$1$invocationCallbackField = null;
		this.$1$interruptCallbackField = null;
		this.set_openCallback(null);
		this.set_closeCallback(null);
		this.set_heartbeatCallback(null);
		this.set_helloCallback(null);
		this.set_authenticateCallback(null);
		this.set_goodbyeCallback(null);
		this.set_publishCallback(new (ss.makeGenericType(ss.Dictionary$2, [ss.Int32, Function]))());
		this.set_subscribeCallback(new (ss.makeGenericType(ss.Dictionary$2, [ss.Int32, Function]))());
		this.set_unsubscribeCallback(new (ss.makeGenericType(ss.Dictionary$2, [ss.Int32, Function]))());
		this.set_callCallback(new (ss.makeGenericType(ss.Dictionary$2, [ss.Int32, Function]))());
		this.set_cancelCallback(new (ss.makeGenericType(ss.Dictionary$2, [ss.Int32, Function]))());
		this.set_registerCallback(new (ss.makeGenericType(ss.Dictionary$2, [ss.Int32, Function]))());
		this.set_unregisterCallback(new (ss.makeGenericType(ss.Dictionary$2, [ss.Int32, Function]))());
		this.set_eventCallback(new (ss.makeGenericType(ss.Dictionary$2, [ss.Int32, Function]))());
		this.set_invocationCallback(new (ss.makeGenericType(ss.Dictionary$2, [ss.Int32, Function]))());
		this.set_interruptCallback(new (ss.makeGenericType(ss.Dictionary$2, [ss.Int32, Function]))());
	};
	$Kadecot_WampClientCore.__typeName = 'Kadecot.WampClientCore';
	global.Kadecot.WampClientCore = $Kadecot_WampClientCore;
	////////////////////////////////////////////////////////////////////////////////
	// Kadecot.WampDeserializer
	var $Kadecot_WampDeserializer = function() {
	};
	$Kadecot_WampDeserializer.__typeName = 'Kadecot.WampDeserializer';
	$Kadecot_WampDeserializer.parse = function(msg) {
		var data;
		if (ss.isNullOrUndefined(msg) || ss.equalsT(msg, '')) {
			throw new ss.ArgumentNullException();
		}
		data = JSON.parse(msg);
		if (!ss.isInstanceOfType(data, ss.IList) || ss.isInstanceOfType(data, ss.IList) && ss.cast(data, Array).length < 2) {
			return null;
		}
		var list = ss.cast(data, Array);
		switch (ss.unbox(ss.cast(list[0], ss.Int32))) {
			case 1: {
				// WAMP SPEC: [HELLO, Realm|uri, Details|dict]
				if (ss.cast(data, Array).length < 3) {
					return null;
				}
				break;
			}
			case 2: {
				// WAMP SPEC: [WELCOME, Session|id, Details|dict]
				if (ss.cast(data, Array).length < 3) {
					return null;
				}
				break;
			}
			case 3: {
				// WAMP SPEC: [ABORT, Details|dict, Reason|uri]
				if (ss.cast(data, Array).length < 3) {
					return null;
				}
				break;
			}
			case 4: {
				// advanced WAMP SPEC: [CHALLENGE, AuthMethod|string, Extra|dict]
				if (ss.cast(data, Array).length < 3) {
					return null;
				}
				break;
			}
			case 5: {
				// advanced WAMP SPEC: [CHALLENGE, AuthMethod|string, Extra|dict]
				if (ss.cast(data, Array).length < 3) {
					return null;
				}
				break;
			}
			case 6: {
				// WAMP SPEC: [GOODBYE, Details|dict, Reason|uri]
				if (ss.cast(data, Array).length < 3) {
					return null;
				}
				break;
			}
			case 7: {
				// advanced WAMP SPEC: [AUTHENTICATE, Signature|string, Extra|dict]
				if (ss.cast(data, Array).length < 3) {
					return null;
				}
				break;
			}
			case 8: {
				// WAMP SPEC: [ERROR, REQUEST.Type|int, REQUEST.Request|id, Details|dict, Error|uri, (Arguments|list, ArgumentsKw|dict)]
				if (ss.cast(data, Array).length < 5) {
					return null;
				}
				switch (ss.unbox(ss.cast(ss.cast(data, Array)[1], ss.Int32))) {
					case 6:
					case 70:
					case 8:
					case 32:
					case 34:
					case 16:
					case 68:
					case 48: {
						// add QUnit test
						// add QUnit test
						// add QUnit test
						break;
					}
					case 64:
					case 66: {
						// WAMP SPEC: [ERROR, REGISTER, REGISTER.Request|id, Details|dict, Error|uri]
						break;
					}
					default: {
						return null;
					}
				}
				break;
			}
			case 16: {
				// WAMP SPEC: [PUBLISH, Request|id, Options|dict, Topic|uri]
				// WAMP SPEC: [PUBLISH, Request|id, Options|dict, Topic|uri, Arguments|list]
				// WAMP SPEC: [PUBLISH, Request|id, Options|dict, Topic|uri, Arguments|list, ArgumentsKw|dict]
				if (ss.cast(data, Array).length < 4) {
					return null;
				}
				break;
			}
			case 17: {
				// WAMP SPEC: [PUBLISHED, PUBLISH.Request|id, Publication|id]
				if (ss.cast(data, Array).length < 3) {
					return null;
				}
				break;
			}
			case 32: {
				// WAMP SPEC: [SUBSCRIBE, Request|id, Options|dict, Topic|uri]
				if (ss.cast(data, Array).length < 4) {
					return null;
				}
				break;
			}
			case 33: {
				// WAMP SPEC: [SUBSCRIBED, SUBSCRIBE.Request|id, Subscription|id]
				if (ss.cast(data, Array).length < 3) {
					return null;
				}
				break;
			}
			case 34: {
				// WAMP SPEC: [UNSUBSCRIBE, Request|id, SUBSCRIBED.Subscription|id]
				if (ss.cast(data, Array).length < 3) {
					return null;
				}
				break;
			}
			case 35: {
				// WAMP SPEC: [UNSUBSCRIBED, UNSUBSCRIBE.Request|id]
				if (ss.cast(data, Array).length < 2) {
					return null;
				}
				break;
			}
			case 36: {
				// WAMP SPEC: [EVENT, SUBSCRIBED.Subscription|id, PUBLISHED.Publication|id, Details|dict]
				// WAMP SPEC: [EVENT, SUBSCRIBED.Subscription|id, PUBLISHED.Publication|id, Details|dict, PUBLISH.Arguments|list]
				// WAMP SPEC: [EVENT, SUBSCRIBED.Subscription|id, PUBLISHED.Publication|id, Details|dict, PUBLISH.Arguments|list, PUBLISH.ArgumentKw|dict]
				if (ss.cast(data, Array).length < 4) {
					return null;
				}
				break;
			}
			case 48: {
				// WAMP SPEC: [CALL, Request|id, Options|dict, Procedure|uri]
				// WAMP SPEC: [CALL, Request|id, Options|dict, Procedure|uri, Arguments|list]
				// WAMP SPEC: [CALL, Request|id, Options|dict, Procedure|uri, Arguments|list, ArgumentsKw|dict]
				if (ss.cast(data, Array).length < 4) {
					return null;
				}
				break;
			}
			case 49: {
				// advanced WAMP SPEC: [CANCEL, CALL.Request|id, Options|dict]
				if (ss.cast(data, Array).length < 3) {
					return null;
				}
				break;
			}
			case 50: {
				// WAMP SPEC: [RESULT, CALL.Request|id, Details|dict]
				// WAMP SPEC: [RESULT, CALL.Request|id, Details|dict, YIELD.Arguments|list]
				// WAMP SPEC: [RESULT, CALL.Request|id, Details|dict, YIELD.Arguments|list, YIELD.ArgumentsKw|dict]
				if (ss.cast(data, Array).length < 3) {
					return null;
				}
				break;
			}
			case 64: {
				// WAMP SPEC: [REGISTER, Request|id, Options|dict, Procedure|uri]
				if (ss.cast(data, Array).length < 4) {
					return null;
				}
				break;
			}
			case 65: {
				// WAMP SPEC: [REGISTERED, REGISTER.Request|id, Registration|id]
				if (ss.cast(data, Array).length < 3) {
					return null;
				}
				break;
			}
			case 66: {
				// WAMP SPEC: [UNREGISTER, Request|id, REGISTERED.Registration|id]
				if (ss.cast(data, Array).length < 3) {
					return null;
				}
				break;
			}
			case 67: {
				// WAMP SPEC: [UNREGISTERED, UNREGISTER.Request|id]
				if (ss.cast(data, Array).length < 2) {
					return null;
				}
				break;
			}
			case 68: {
				// WAMP SPEC: [INVOCATION, Request|id, REGISTERED.Registration|id, Details|dict]
				// WAMP SPEC: [INVOCATION, Request|id, REGISTERED.Registration|id, Details|dict, CALL.Arguments|list]
				// WAMP SPEC: [INVOCATION, Request|id, REGISTERED.Registration|id, Details|dict, CALL.Arguments|list, CALL.ArgumentsKw|dict]
				if (ss.cast(data, Array).length < 4) {
					return null;
				}
				break;
			}
			case 69: {
				// advanced WAMP SPEC: [INTERRUPT, INVOCATION.Request|id, Options|dict]
				if (ss.cast(data, Array).length < 3) {
					return null;
				}
				break;
			}
			case 70: {
				// WAMP SPEC: [YIELD, INVOCATION.Request|id, Options|dict, (Arguments|list, ArgumentsKw|dict)]
				if (ss.cast(data, Array).length < 3) {
					return null;
				}
				break;
			}
			default: {
				return null;
			}
		}
		return data;
	};
	global.Kadecot.WampDeserializer = $Kadecot_WampDeserializer;
	////////////////////////////////////////////////////////////////////////////////
	// Kadecot.WampSerializer
	var $Kadecot_WampSerializer = function() {
	};
	$Kadecot_WampSerializer.__typeName = 'Kadecot.WampSerializer';
	$Kadecot_WampSerializer.createHello = function(realm, details) {
		var data;
		if (ss.isNullOrUndefined(realm) || ss.equalsT(realm, '')) {
			throw new ss.ArgumentNullException();
		}
		if (ss.isNullOrUndefined(details) || ss.equalsT(details, '')) {
			throw new ss.ArgumentNullException();
		}
		var detailJson = JSON.parse(details);
		var list = [];
		ss.add(list, 1);
		ss.add(list, realm);
		ss.add(list, detailJson);
		data = JSON.stringify(list);
		return data;
	};
	$Kadecot_WampSerializer.createWelcome = function(session, details) {
		var data;
		var detailJson = JSON.parse(details);
		var list = [];
		ss.add(list, 2);
		ss.add(list, session);
		ss.add(list, detailJson);
		data = JSON.stringify(list);
		return data;
	};
	$Kadecot_WampSerializer.createAbort = function(details, reason) {
		var data;
		if (ss.isNullOrUndefined(details) || ss.equalsT(details, '')) {
			throw new ss.ArgumentNullException();
		}
		if (ss.isNullOrUndefined(reason) || ss.equalsT(reason, '')) {
			throw new ss.ArgumentNullException();
		}
		var detailJson = JSON.parse(details);
		var list = [];
		ss.add(list, 3);
		ss.add(list, detailJson);
		ss.add(list, reason);
		data = JSON.stringify(list);
		return data;
	};
	$Kadecot_WampSerializer.createChallenge = function(authMethod, extra) {
		var data;
		if (ss.isNullOrUndefined(authMethod) || ss.equalsT(authMethod, '')) {
			throw new ss.ArgumentNullException();
		}
		if (ss.isNullOrUndefined(extra) || ss.equalsT(extra, '')) {
			throw new ss.ArgumentNullException();
		}
		var extraJson = JSON.parse(extra);
		var list = [];
		ss.add(list, 4);
		ss.add(list, authMethod);
		ss.add(list, extraJson);
		data = JSON.stringify(list);
		return data;
	};
	$Kadecot_WampSerializer.createAuthenticate = function(signature, extra) {
		var data;
		if (ss.isNullOrUndefined(signature) || ss.equalsT(signature, '')) {
			throw new ss.ArgumentNullException();
		}
		if (ss.isNullOrUndefined(extra) || ss.equalsT(extra, '')) {
			throw new ss.ArgumentNullException();
		}
		var extraJson = JSON.parse(extra);
		var list = [];
		ss.add(list, 5);
		ss.add(list, signature);
		ss.add(list, extraJson);
		data = JSON.stringify(list);
		return data;
	};
	$Kadecot_WampSerializer.createGoodbye = function(details, reason) {
		var data;
		if (ss.isNullOrUndefined(details) || ss.equalsT(details, '')) {
			throw new ss.ArgumentNullException();
		}
		if (ss.isNullOrUndefined(reason)) {
			throw new ss.ArgumentNullException();
		}
		var detailJson = JSON.parse(details);
		var list = [];
		ss.add(list, 6);
		ss.add(list, detailJson);
		ss.add(list, reason);
		data = JSON.stringify(list);
		return data;
	};
	$Kadecot_WampSerializer.createHeartbeat = function(incomingSeq, outgoingSeq, discard) {
		var data;
		var list = [];
		if (ss.isNullOrUndefined(discard) || ss.equalsT(discard, '')) {
			ss.add(list, 7);
			ss.add(list, incomingSeq);
			ss.add(list, outgoingSeq);
			data = JSON.stringify(list);
		}
		else {
			ss.add(list, 7);
			ss.add(list, incomingSeq);
			ss.add(list, outgoingSeq);
			ss.add(list, discard);
			data = JSON.stringify(list);
		}
		return data;
	};
	$Kadecot_WampSerializer.createError = function(type, request, details, error, arguments1, argumentsKw) {
		var data;
		if (ss.isNullOrUndefined(details) || ss.equalsT(details, '')) {
			throw new ss.ArgumentNullException();
		}
		if (ss.isNullOrUndefined(error) || ss.equalsT(error, '')) {
			throw new ss.ArgumentNullException();
		}
		var detailJson = JSON.parse(details);
		var argsJson = JSON.parse(arguments1);
		var argsKwJson = JSON.parse(argumentsKw);
		var list = [];
		ss.add(list, 8);
		ss.add(list, type);
		ss.add(list, request);
		ss.add(list, detailJson);
		ss.add(list, error);
		if (ss.isValue(arguments1) && ss.isNullOrUndefined(argumentsKw)) {
			ss.add(list, argsJson);
		}
		else if (ss.isValue(arguments1) && ss.isValue(argumentsKw)) {
			ss.add(list, argsJson);
			ss.add(list, argsKwJson);
		}
		data = JSON.stringify(list);
		return data;
	};
	$Kadecot_WampSerializer.createPublish = function(request, options, topic, arguments1, argumentsKw) {
		var data;
		if (ss.isNullOrUndefined(options) || ss.equalsT(options, '')) {
			throw new ss.ArgumentNullException();
		}
		if (ss.isNullOrUndefined(topic) || ss.equalsT(topic, '')) {
			throw new ss.ArgumentNullException();
		}
		var optJson = JSON.parse(options);
		var argsJson = JSON.parse(arguments1);
		var argsKwJson = JSON.parse(argumentsKw);
		var list = [];
		ss.add(list, 16);
		ss.add(list, request);
		ss.add(list, optJson);
		ss.add(list, topic);
		if (ss.isValue(arguments1) && ss.isNullOrUndefined(argumentsKw)) {
			ss.add(list, argsJson);
		}
		else if (ss.isValue(arguments1) && ss.isValue(argumentsKw)) {
			ss.add(list, argsJson);
			ss.add(list, argsKwJson);
		}
		data = JSON.stringify(list);
		return data;
	};
	$Kadecot_WampSerializer.createPublished = function(request, publication) {
		var data;
		var list = [];
		ss.add(list, 17);
		ss.add(list, request);
		ss.add(list, publication);
		data = JSON.stringify(list);
		return data;
	};
	$Kadecot_WampSerializer.createSubscribe = function(request, options, topic) {
		var data;
		if (ss.isNullOrUndefined(options) || ss.equalsT(options, '')) {
			throw new ss.ArgumentNullException();
		}
		if (ss.isNullOrUndefined(topic) || ss.equalsT(topic, '')) {
			throw new ss.ArgumentNullException();
		}
		var optJson = JSON.parse(options);
		var list = [];
		ss.add(list, 32);
		ss.add(list, request);
		ss.add(list, optJson);
		ss.add(list, topic);
		data = JSON.stringify(list);
		return data;
	};
	$Kadecot_WampSerializer.createSubscribed = function(request, subscription) {
		var data;
		var list = [];
		ss.add(list, 33);
		ss.add(list, request);
		ss.add(list, subscription);
		data = JSON.stringify(list);
		return data;
	};
	$Kadecot_WampSerializer.createUnsubscribe = function(request, subscription) {
		var data;
		var list = [];
		ss.add(list, 34);
		ss.add(list, request);
		ss.add(list, subscription);
		//			list.Add ("com.sonycsl.kadecot.echonetlite.procedure.set.topic.HomeAirConditioner.OperationStatus");
		data = JSON.stringify(list);
		return data;
	};
	$Kadecot_WampSerializer.createUnsubscribed = function(request) {
		var data;
		var list = [];
		ss.add(list, 35);
		ss.add(list, request);
		data = JSON.stringify(list);
		return data;
	};
	$Kadecot_WampSerializer.createEvent = function(subscription, publication, details, arguments1, argumentsKw) {
		var data;
		if (ss.isNullOrUndefined(details) || ss.equalsT(details, '')) {
			throw new ss.ArgumentNullException();
		}
		var detailJson = JSON.parse(details);
		var argsJson = JSON.parse(arguments1);
		var argsKwJson = JSON.parse(argumentsKw);
		var list = [];
		ss.add(list, 36);
		ss.add(list, subscription);
		ss.add(list, publication);
		ss.add(list, detailJson);
		if (ss.isValue(arguments1) && ss.isNullOrUndefined(argumentsKw)) {
			ss.add(list, argsJson);
		}
		else if (ss.isValue(arguments1) && ss.isValue(argumentsKw)) {
			ss.add(list, argsJson);
			ss.add(list, argsKwJson);
		}
		data = JSON.stringify(list);
		return data;
	};
	$Kadecot_WampSerializer.createCall = function(request, options, procedure, arguments1, argumentsKw) {
		var data;
		if (ss.isNullOrUndefined(options) || ss.equalsT(options, '')) {
			throw new ss.ArgumentNullException();
		}
		if (ss.isNullOrUndefined(procedure) || ss.equalsT(procedure, '')) {
			throw new ss.ArgumentNullException();
		}
		var optionJson = JSON.parse(options);
		var argsJson = JSON.parse(arguments1);
		var argsKwJson = JSON.parse(argumentsKw);
		var list = [];
		ss.add(list, 48);
		ss.add(list, request);
		ss.add(list, optionJson);
		ss.add(list, procedure);
		if (ss.isValue(arguments1) && ss.isNullOrUndefined(argumentsKw)) {
			ss.add(list, argsJson);
		}
		else if (ss.isValue(arguments1) && ss.isValue(argumentsKw)) {
			ss.add(list, argsJson);
			ss.add(list, argsKwJson);
		}
		data = JSON.stringify(list);
		return data;
	};
	$Kadecot_WampSerializer.createCancel = function(request, options) {
		var data;
		if (ss.isNullOrUndefined(options) || ss.equalsT(options, '')) {
			throw new ss.ArgumentNullException();
		}
		var optionJson = JSON.parse(options);
		var list = [];
		ss.add(list, 49);
		ss.add(list, request);
		ss.add(list, optionJson);
		data = JSON.stringify(list);
		return data;
	};
	$Kadecot_WampSerializer.createResult = function(request, details, arguments1, argumentsKw) {
		var data;
		if (ss.isNullOrUndefined(details) || ss.equalsT(details, '')) {
			throw new ss.ArgumentNullException();
		}
		var detailJson = JSON.parse(details);
		var argsJson = JSON.parse(arguments1);
		var argsKwJson = JSON.parse(argumentsKw);
		var list = [];
		ss.add(list, 50);
		ss.add(list, request);
		ss.add(list, detailJson);
		if (ss.isValue(arguments1) && ss.isNullOrUndefined(argumentsKw)) {
			ss.add(list, argsJson);
		}
		else if (ss.isValue(arguments1) && ss.isValue(argumentsKw)) {
			ss.add(list, argsJson);
			ss.add(list, argsKwJson);
		}
		data = JSON.stringify(list);
		return data;
	};
	$Kadecot_WampSerializer.createRegister = function(request, options, procedure) {
		var data;
		if (ss.isNullOrUndefined(options) || ss.equalsT(options, '')) {
			throw new ss.ArgumentNullException();
		}
		if (ss.isNullOrUndefined(procedure) || ss.equalsT(procedure, '')) {
			throw new ss.ArgumentNullException();
		}
		var optJson = JSON.parse(options);
		var list = [];
		ss.add(list, 64);
		ss.add(list, request);
		ss.add(list, optJson);
		ss.add(list, procedure);
		data = JSON.stringify(list);
		return data;
	};
	$Kadecot_WampSerializer.createRegistered = function(request, registration) {
		var data;
		var list = [];
		ss.add(list, 65);
		ss.add(list, request);
		ss.add(list, registration);
		data = JSON.stringify(list);
		return data;
	};
	$Kadecot_WampSerializer.createUnregister = function(request, registration) {
		var data;
		var list = [];
		ss.add(list, 66);
		ss.add(list, request);
		ss.add(list, registration);
		data = JSON.stringify(list);
		return data;
	};
	$Kadecot_WampSerializer.createUnregistered = function(request) {
		var data;
		var list = [];
		ss.add(list, 67);
		ss.add(list, request);
		data = JSON.stringify(list);
		return data;
	};
	$Kadecot_WampSerializer.createInvocation = function(request, registration, details, arguments1, argumentsKw) {
		var data;
		if (ss.isNullOrUndefined(details) || ss.equalsT(details, '')) {
			throw new ss.ArgumentNullException();
		}
		var detailJson = JSON.parse(details);
		var argsJson = JSON.parse(arguments1);
		var argsKwJson = JSON.parse(argumentsKw);
		var list = [];
		ss.add(list, 68);
		ss.add(list, request);
		ss.add(list, registration);
		ss.add(list, detailJson);
		if (ss.isValue(arguments1) && ss.isNullOrUndefined(argumentsKw)) {
			ss.add(list, argsJson);
		}
		else if (ss.isValue(arguments1) && ss.isValue(argumentsKw)) {
			ss.add(list, argsJson);
			ss.add(list, argsKwJson);
		}
		data = JSON.stringify(list);
		return data;
	};
	$Kadecot_WampSerializer.createInterrupt = function(request, options) {
		var data;
		if (ss.isNullOrUndefined(options) || ss.equalsT(options, '')) {
			throw new ss.ArgumentNullException();
		}
		var optJson = JSON.parse(options);
		var list = [];
		ss.add(list, 69);
		ss.add(list, request);
		ss.add(list, optJson);
		data = JSON.stringify(list);
		return data;
	};
	$Kadecot_WampSerializer.createYield = function(request, options, arguments1, argumentsKw) {
		var data;
		if (ss.isNullOrUndefined(options) || ss.equalsT(options, '')) {
			throw new ss.ArgumentNullException();
		}
		var optJson = JSON.parse(options);
		var argsJson = JSON.parse(arguments1);
		var argsKwJson = JSON.parse(argumentsKw);
		var list = [];
		ss.add(list, 70);
		ss.add(list, request);
		ss.add(list, optJson);
		if (ss.isValue(arguments1) && ss.isNullOrUndefined(argumentsKw)) {
			ss.add(list, argsJson);
		}
		else if (ss.isValue(arguments1) && ss.isValue(argumentsKw)) {
			ss.add(list, argsJson);
			ss.add(list, argsKwJson);
		}
		data = JSON.stringify(list);
		return data;
	};
	global.Kadecot.WampSerializer = $Kadecot_WampSerializer;
	ss.initClass($CtrlUI, $asm, {
		get_$m_CurrentNode: function() {
			return this.$_m_CurrentNode;
		},
		set_$m_CurrentNode: function(value) {
			var flag = !ss.referenceEquals(this.$_m_CurrentNode, value) || ss.isNullOrUndefined(this.$_m_CurrentNode) && ss.isNullOrUndefined(value);
			this.$_m_CurrentNode = value;
			if (flag) {
				$(document).trigger($control_UIEventNames.currentNodeChange);
			}
		},
		$initClassGroups$1: function(eve) {
			$CtrlUI.$page = eve.target;
			this.$setEvent();
			if (ss.isValue($CtrlUI.$m_ClassGroups)) {
				return;
			}
			$ctrlui_DeviceController.getClassInfo = ss.mkdel(this, this.$getClassInfoHandler);
			this.set_$m_CurrentNode(null);
			var ClassGroupListUrl = 'devices/ClassGroupList.json';
			$.ajax(ClassGroupListUrl).success(ss.mkdel(this, this.$initClassGroups)).error(function(request, textStatus, error) {
				$CtrlUI.$ajaxError(request, textStatus, error, ClassGroupListUrl);
			});
		},
		$setEvent: function() {
			$(document).on($control_UIEventNames.loadingShow, $control_UIEventHandler.loadingShow);
			$(document).on($control_UIEventNames.loadingHide, $control_UIEventHandler.loadingHide);
			$(document).on($control_UIEventNames.currentNodeChange, ss.thisFix(ss.mkdel(this, this.$currentNodeChange)));
			$(document).on($control_UIEventNames.searchStart, ss.mkdel(this, this.searchStart));
			$(document).on($control_UIEventNames.searchEnd, ss.mkdel(this, this.searchEnd));
			$(document).on($control_UIEventNames.searchError, ss.mkdel(this, this.searchError));
			$(document).on($control_UIEventNames.propertyWriteStart, ss.mkdel(this, this.propertyWriteStart));
			$(document).on($control_UIEventNames.propertyWriteEnd, ss.mkdel(this, this.propertyWriteEnd));
			this.get_connectionSwt().change(ss.thisFix(ss.mkdel(this, this.connect)));
			this.get_setPropertyBtn().click(ss.thisFix(ss.mkdel(this, this.setProperty)));
			this.get_getPropertyBtn().click(ss.mkdel(this, this.getProperty));
			this.get_setGetPropertyBtn().click(ss.thisFix(ss.mkdel(this, this.setGetProperty)));
			this.get_kadecotBtn().click(ss.thisFix(ss.mkdel(this, this.kadecotClick)));
			this.get_propertyList().change(ss.thisFix(ss.mkdel(this, this.propertyChange)));
			this.get_setInitParamsBtn().click(ss.thisFix(ss.mkdel(this, this.$setInitParams)));
			this.get_getInitParamsBtn().click(ss.thisFix(ss.mkdel(this, this.$getInitParams)));
		},
		$initClassGroups: function(data) {
			var classGroups = $control_UI.toJSClassGroupInfoList(data);
			//初期化
			$CtrlUI.$m_Classes = [];
			$CtrlUI.$m_ClassGroups = Array.prototype.slice.call(classGroups);
			this.$getClassInfoHandler($ctrlui_T_ECN_EOJ.x1_PROFILE, $ctrlui_T_ECN_EOJ.x2_NODE_PROFILE, ss.mkdel(this, this.$getClassInfoCallback));
		},
		$getClassInfoCallback: function(di) {
			$CtrlUI.$m_ProfileClassInfo = di;
		},
		$getClassInfoHandler: function(x1, x2, cb) {
			var eclass = Enumerable.from($CtrlUI.$m_Classes).firstOrDefault(function(p) {
				return p.classGroup.classGroupCode === x1 && p.classCode === x2;
			}, ss.getDefaultValue($ctrlui_JsonClassInfo));
			//すでにjsonファイルが読み込まれているので、ファイルを読み込まない。
			if (ss.isValue(eclass)) {
				if (ss.isNullOrUndefined(eclass.properties)) {
					this.$getPropertyList(eclass, cb);
				}
				else {
					cb(eclass);
				}
				return;
			}
			//eclass==nullの場合
			var classGroup = Enumerable.from($CtrlUI.$m_ClassGroups).firstOrDefault(function(p1) {
				return p1.classGroupCode === x1;
			}, ss.getDefaultValue($ctrlui_JsonClassGroupInfo));
			var classListUrl = 'devices/ClassList' + ss.formatNumber(x1, 'X2').toUpperCase() + '.json';
			$.ajax(classListUrl).success(ss.mkdel(this, function(data) {
				this.$getClassInfoSuccess(data, x1, x2, classGroup, cb);
			})).error(ss.mkdel(this, function(request, textStatus, error) {
				this.$getClassInfoError(request, textStatus, error, eclass, cb, classListUrl);
			}));
		},
		$getClassInfoSuccess: function(data, x1, x2, classGroup, cb) {
			var classes = $control_UI.toJSClassInfoList(data, classGroup);
			ss.arrayAddRange($CtrlUI.$m_Classes, classes);
			var eclass = Enumerable.from($CtrlUI.$m_Classes).firstOrDefault(function(p) {
				return p.classGroup.classGroupCode === x1 && p.classCode === x2;
			}, ss.getDefaultValue($ctrlui_JsonClassInfo));
			this.$getPropertyList(eclass, cb);
		},
		$getPropertyList: function(eclass, cb) {
			var PropertyListUrl = 'devices/PropertyList' + ss.formatNumber(eclass.classGroup.classGroupCode, 'X2').toUpperCase() + ss.formatNumber(eclass.classCode, 'X2').toUpperCase() + '.json';
			$.ajax(PropertyListUrl).success(ss.mkdel(this, function(data1) {
				this.$getPropertyListSuccess(data1, eclass);
			})).error(ss.mkdel(this, function(request, textStatus, error) {
				this.$propertyListAjaxError(request, textStatus, error, eclass, cb, PropertyListUrl);
			})).complete(ss.mkdel(this, function(request1, textStatus1) {
				this.$getPropertyListComplete(request1, textStatus1, eclass, cb);
			}));
		},
		$getPropertyListSuccess: function(data, eclass) {
			var properties = $control_UI.toJSPropertyInfoList(data);
			eclass.properties = Array.prototype.slice.call(properties);
		},
		$getPropertyListComplete: function(request, textStatus, eclass, cb) {
			if (ss.equalsT(request.statusText, 'OK')) {
				cb(eclass);
			}
		},
		$getClassInfoError: function(request, textStatus, error, eclass, cb, url) {
			$CtrlUI.$ajaxError(request, textStatus, error, url);
			cb(eclass);
		},
		$propertyListAjaxError: function(request, textStatus, error, eclass, cb, url) {
			var PropertyListBaseUrl = 'devices/PropertyListBase.json';
			$.ajax(PropertyListBaseUrl).success(ss.mkdel(this, function(data1) {
				this.$getPropertyListSuccess(data1, eclass);
			})).error(function(request1, textStatus1, error1) {
				$CtrlUI.$ajaxError(request1, textStatus1, error1, url);
			}).complete(ss.mkdel(this, function(request11, textStatus11) {
				this.$getPropertyListComplete(request11, textStatus11, eclass, cb);
			}));
		},
		$sendHello: function() {
			this.$m_WampClient.sendHello('default', '{"roles":{"caller":{},"subscriber":{}}}', ss.mkdel(this, this.$onWelcom));
		},
		$onWelcom: function(args) {
			this.$m_WampClient.sendSubscribe('{}', 'com.sonycsl.kadecot.echonetlite.topic.GeneralLighting.OperationStatus', ss.mkdel(this, this.$onSubscribed), ss.mkdel(this, this.$onSubscribed));
		},
		$onSubscribed: function(args) {
		},
		$onDevListed: function(args) {
			var cmd = ss.unbox(ss.cast(ss.cast(args, Array)[0], ss.Int32));
			if (cmd !== 50) {
				$(document).trigger($control_UIEventNames.searchError);
				return;
			}
			//初期化
			var alldevs = $control_UI.getAllDevices(args);
			this.$getDeviceInfo(alldevs, 0, []);
		},
		$getDeviceInfo: function(alldevs, pos, m_NodeList) {
			var device = alldevs[pos];
			var callback = ss.mkdel(this, function(args) {
				var cmd = ss.unbox(ss.cast(ss.cast(args, Array)[0], ss.Int32));
				if (cmd !== 50) {
					$(document).trigger($control_UIEventNames.searchError);
					return;
				}
				var dev = new $ctrlui_WebApiObjectInfo(device.deviceId, ss.cast(args, Array)[4], false);
				this.$getClassInfoHandler(dev.x1, dev.x2, ss.mkdel(this, function(dc) {
					var di = $control_UI.getDeviceInfo(dc, dev, device);
					var node = Enumerable.from(m_NodeList).firstOrDefault(function(ni) {
						return ss.referenceEquals(ss.cast(ni.get_data(), String), device.ip_addr);
					}, ss.getDefaultValue($ctrlui_DeviceController$NodeInfo));
					if (ss.isNullOrUndefined(node)) {
						var profile = new $ctrlui_DeviceController$DeviceInfo.$ctor2($CtrlUI.$m_ProfileClassInfo, true, $ctrlui_T_ECN_EOJ.x1_PROFILE, $ctrlui_T_ECN_EOJ.x2_NODE_PROFILE, 1, dev.enodid);
						node = new $ctrlui_DeviceController$NodeInfo(dev.enodid, profile);
						node.set_data(device.ip_addr);
						m_NodeList.push(node);
					}
					node.get_devices().push(di);
					pos++;
					if (pos < alldevs.length) {
						this.$getDeviceInfo(alldevs, pos, m_NodeList);
					}
					else {
						$(document).trigger($control_UIEventNames.searchEnd);
						this.$m_DeviceController = new $ctrlui_DeviceController(m_NodeList);
						this.$updateUI();
					}
				}));
			});
			if (device.protocol === 'arduino') {
				var arg = { x1: 14, x2: 240, x3: 1, enodid: 1, properties: [{ epc: 0, flag: 'RULE_SET,RULE_GET', size: 1 }, { epc: 1, flag: 'RULE_SET,RULE_GET', size: 1 }, { epc: 2, flag: 'RULE_SET,RULE_GET', size: 1 }, { epc: 3, flag: 'RULE_SET,RULE_GET', size: 1 }, { epc: 4, flag: 'RULE_SET,RULE_GET', size: 1 }, { epc: 5, flag: 'RULE_SET,RULE_GET', size: 1 }, { epc: 6, flag: 'RULE_SET,RULE_GET', size: 1 }, { epc: 7, flag: 'RULE_SET,RULE_GET', size: 1 }, { epc: 8, flag: 'RULE_SET,RULE_GET', size: 1 }, { epc: 9, flag: 'RULE_SET,RULE_GET', size: 1 }, { epc: 10, flag: 'RULE_SET,RULE_GET', size: 1 }, { epc: 11, flag: 'RULE_SET,RULE_GET', size: 1 }, { epc: 12, flag: 'RULE_SET,RULE_GET', size: 1 }, { epc: 13, flag: 'RULE_SET,RULE_GET', size: 1 }, { epc: 14, flag: 'RULE_GET', size: 1 }, { epc: 15, flag: 'RULE_GET', size: 1 }, { epc: 16, flag: 'RULE_GET', size: 1 }, { epc: 17, flag: 'RULE_GET', size: 1 }, { epc: 18, flag: 'RULE_GET', size: 1 }, { epc: 19, flag: 'RULE_GET', size: 1 }] };
				callback([50, 0, { deviceId: 1 }, [], arg]);
			}
			else {
				this.$m_WampClient.sendCall('{"deviceId":' + device.deviceId + '}', 'jp.toppers.ecnl.procedure.getDeviceInfo', null, null, callback);
			}
		},
		$updateUI: function() {
			if (ss.isNullOrUndefined(this.get_nodeList())) {
				return;
			}
			this.get_nodeList().html('');
			this.get_nodeList().append(this.$m_DeviceController.getUINodeList(ss.mkdel(this, this.$listitem_Click)));
			this.get_nodeList().collapsibleset({}).collapsibleset('refresh');
			$("ul[data-role='listview']", this.get_nodeList()).listview().listview('refresh');
		},
		$onResponse: function(args) {
			var switchData = ss.unbox(ss.cast(ss.cast(args, Array)[0], ss.Int32));
			if (switchData !== 50) {
				return;
			}
			var doc = ss.cast(args, Array)[4];
			if (ss.isValue(this.$m_DeviceController)) {
				this.$m_DeviceController.recvResponse(new $ctrlui_WebApiEchonetMessage(doc));
				if (ss.isNullOrUndefined(this.get_propertyList())) {
					return;
				}
				this.get_$m_CurrentNode().getData();
			}
			$control_UI.disable(this.get_setPropertyBtn(), true);
		},
		$onSetResponse: function(args) {
			var doc = ss.cast(args, Array)[4];
			if (ss.isValue(this.$m_DeviceController)) {
				if (ss.isNullOrUndefined(this.get_propertyList())) {
					return;
				}
				this.$m_DeviceController.recvResponse(new $ctrlui_WebApiEchonetMessage(doc));
			}
			$control_UI.disable(this.get_setPropertyBtn(), true);
		},
		$listitem_Click: function(elem, ev, di) {
			if (ss.isNullOrUndefined(this.get_propertyList())) {
				return;
			}
			this.set_$m_CurrentNode(di);
		},
		$wampClientConnected: function() {
			//接続スイッチがOFFの時、スイッチを切り替える
			var flag = this.get_connectionSwt().prop('checked');
			if (!flag) {
				this.get_connectionSwt().prop('checked', true).flipswitch('refresh');
			}
			$control_UI.toggleClass(this.get_searchBtn(), 'ui-icon-alert', 'ui-icon-refresh');
			this.$sendHello();
		},
		$wampClientDisconnected: function() {
			$control_UI.toggleClass(this.get_searchBtn(), 'ui-icon-refresh', 'ui-icon-alert');
		},
		connect: function(ele, eve) {
			var connect = $(ele).prop('checked');
			//接続スイッチがOn && m_WampClient.isConnecting() == false
			if (connect && !this.$m_WampClient.isConnecting()) {
				this.$m_WampClient.connect('ws://' + window.location.host + '/webapi');
			}
			else if (!connect && this.$m_WampClient.isConnecting()) {
				this.$m_WampClient.disconnect();
				//  m_WampClient.sendGoodbye("{}", "wamp.error.system_shutdown",Disconnect);
			}
		},
		disconnect: function(args) {
			this.$m_WampClient.disconnect();
		},
		search: function(ele, ev) {
			$(document).trigger($control_UIEventNames.searchStart);
			var data = this.$m_WampClient.sendCall('{}', 'com.sonycsl.kadecot.provider.procedure.getDeviceList', null, null, ss.mkdel(this, this.$onDevListed));
			if (ss.isNullOrUndefined(data)) {
				$(document).trigger($control_UIEventNames.searchError);
				return;
			}
			this.$timerId = window.setInterval(function() {
				$(document).trigger($control_UIEventNames.searchError);
			}, 20000);
		},
		searchStart: function(ev) {
			$.mobile.loading('show', new $LoadingOption.$ctor1('機器検索中...', true));
			$control_UI.disable(this.get_searchBtn(), true);
		},
		searchEnd: function(ev) {
			if (this.$timerId !== 0) {
				window.clearInterval(this.$timerId);
				this.$timerId = 0;
			}
			$.mobile.loading('hide');
			$control_UI.disable(this.get_searchBtn(), false);
			$control_UI.toggleClass(this.get_nodeList().find('li .ui-icon-alert'), 'ui-icon-alert', 'ui-icon-carat-r');
		},
		searchError: function(ev) {
			if (this.$timerId !== 0) {
				window.clearInterval(this.$timerId);
				this.$timerId = 0;
			}
			$.mobile.loading('hide');
			$('#bt_search').removeAttr('disabled');
			window.alert('機器が見つかりませんでした');
			$control_UI.toggleClass(this.get_nodeList().find('li .ui-icon-carat-r'), 'ui-icon-carat-r', 'ui-icon-alert');
		},
		propertyWriteStart: function(ev) {
			$.mobile.loading('show');
		},
		propertyWriteEnd: function(ev) {
			//PropertyList.Find("li").Find(".ui-field-contain").Children(".ui-input-text").Each((i, ele) => {
			//	if (jQuery.Select(ele).Children(".ui-input-text").Length > 0) {
			//		jQuery.Select(ele).Children("a").Remove();
			//		jQuery.Select(ele).Children(".ui-input-text").Unwrap();
			//	}
			//});
			$.mobile.loading('hide');
		},
		setProperty: function(ele, eve) {
			var esv = new $ctrlui_WebApiSet();
			var ctrls = this.get_propertyList();
			esv.deojid = this.get_$m_CurrentNode().get_eObjId();
			var enable = false;
			var $t1 = this.get_$m_CurrentNode().get_properties();
			for (var $t2 = 0; $t2 < $t1.length; $t2++) {
				var dpi = $t1[$t2];
				if (ss.isNullOrUndefined(dpi.get_inputData())) {
					continue;
				}
				else if (dpi.get_isWaitRes()) {
					esv.addEdt(dpi.get_propertyCode(), dpi.get_inputData());
					enable = true;
				}
			}
			if (!enable) {
				return;
			}
			var call = this.$m_WampClient.sendCall(esv.getOption(), 'jp.toppers.ecnl.procedure.set', '[]', esv.getArguments(), ss.mkdel(this, this.$onSetResponse));
			if (ss.isNullOrUndefined(call)) {
				window.alert('機器が接続されていません');
			}
		},
		getProperty: function(eve) {
			var protocol = ss.cast(this.get_$m_CurrentNode().get_data(), $ctrlui_WampApiKadecotDevice).protocol;
			if (protocol === 'arduino') {
				return;
			}
			var esv = new $ctrlui_WebApiGet();
			//現在選択中のノードからEObjIDを取得
			esv.deojid = this.get_$m_CurrentNode().get_eObjId();
			//取得フラグ
			var enable = false;
			var $t1 = this.get_$m_CurrentNode().get_properties();
			for (var $t2 = 0; $t2 < $t1.length; $t2++) {
				var dpi = $t1[$t2];
				if (ss.isNullOrUndefined(dpi)) {
					continue;
				}
				dpi.set_isWaitRes(false);
				var target = (dpi.get_flag() & 2) !== 0;
				//読み込み禁止の場合
				if (!target) {
					dpi.set_errorText('読み込み禁止です');
					continue;
				}
				dpi.set_isWaitRes(true);
				esv.addEpc(dpi.get_propertyCode());
				//読み込み可なのでtrue
				enable = true;
			}
			if (!enable) {
				return;
			}
			var call = this.$m_WampClient.sendCall(esv.getOption(), 'jp.toppers.ecnl.procedure.get', '[]', esv.getArguments(), ss.mkdel(this, this.$onResponse));
			if (ss.isNullOrUndefined(call)) {
				$control_UI.toggleClass($control_UI.getDeviceBtn(this.get_$m_CurrentNode()), 'ui-icon-carat-r', 'ui-icon-alert');
			}
			else {
				$control_UI.toggleClass($control_UI.getDeviceBtn(this.get_$m_CurrentNode()), 'ui-icon-alert', 'ui-icon-carat-r');
			}
		},
		setGetProperty: function(ele, ev) {
			var esv = new $ctrlui_WebApiSet();
			esv.deojid = this.get_$m_CurrentNode().get_eObjId();
			var enable = false;
			var $t1 = this.get_$m_CurrentNode().get_properties();
			for (var $t2 = 0; $t2 < $t1.length; $t2++) {
				var dpi = $t1[$t2];
				if (ss.isNullOrUndefined(dpi.get_inputData())) {
					continue;
				}
				else if (dpi.get_isWaitRes()) {
					esv.addEdt(dpi.get_propertyCode(), dpi.get_inputData());
					enable = true;
				}
			}
			if (!enable) {
				return;
			}
			var call = this.$m_WampClient.sendCall(esv.getOption(), 'jp.toppers.ecnl.procedure.setget', '[]', esv.getArguments(), ss.mkdel(this, this.$onResponse));
			if (ss.isNullOrUndefined(call)) {
				window.alert('機器が接続されていません');
			}
		},
		propertyChange: function(ele, ev) {
			var flag = this.get_$m_CurrentNode().isWait();
			$control_UI.disable(this.get_setPropertyBtn(), !flag);
			$control_UI.disable(this.get_setGetPropertyBtn(), !flag);
		},
		kadecotClick: function(ele, ev) {
			window.open('http://app.kadecot.net/index.html?kip=' + window.location.hostname, 'Kadecot');
		},
		kadecotSet: function(dpi, data) {
			var propName;
			if (dpi.get_propertyCode() === 128) {
				propName = 'OperationStatus';
			}
			else {
				propName = '0x' + dpi.get_propertyCode().toString(16);
			}
			var di = this.get_$m_CurrentNode();
			if (ss.isNullOrUndefined(di)) {
				return;
			}
			this.$dpi = dpi;
			var msg = new $ctrlui_WampApiKadecotSet(di.get_eObjId(), propName, data);
			this.$m_WampClient.sendCall(msg.$getOption(), 'com.sonycsl.kadecot.echonetlite.procedure.set', '[]', msg.$getArguments(), ss.mkdel(this, this.$kadecotSet));
		},
		$kadecotSet: function(args) {
			var di = this.get_$m_CurrentNode();
			var doc = ss.cast(args, Array)[4];
			var res = new $ctrlui_WampApiKadecotRes(doc);
			if (ss.isValue(this.$m_DeviceController)) {
				this.$m_DeviceController.recvResponse$1(di, 113, this.$dpi.get_propertyCode(), res.propertyValue);
			}
		},
		kadecotGet$1: function(dpi) {
			var propName;
			if (dpi.get_propertyCode() === 128) {
				propName = 'OperationStatus';
			}
			else {
				propName = '0x' + dpi.get_propertyCode().toString(16);
			}
			var di = this.get_$m_CurrentNode();
			if (ss.isNullOrUndefined(di)) {
				return;
			}
			var msg = new $ctrlui_WampApiKadecotGet(di.get_eObjId(), propName);
			this.$dpi = dpi;
			this.$m_WampClient.sendCall(msg.$getOption(), 'com.sonycsl.kadecot.echonetlite.procedure.get', '[]', msg.$getArguments(), ss.mkdel(this, this.kadecotGet));
		},
		kadecotGet: function(args) {
			var di = this.get_$m_CurrentNode();
			var doc = ss.cast(args, Array)[4];
			var res = new $ctrlui_WampApiKadecotRes(doc);
			if (ss.isValue(this.$m_DeviceController)) {
				this.$m_DeviceController.recvResponse$1(di, 114, this.$dpi.get_propertyCode(), res.propertyValue);
			}
		},
		$currentNodeChange: function(elem, ev) {
			this.$buttonDisabled(this.get_$m_CurrentNode());
			if (ss.isNullOrUndefined(this.get_$m_CurrentNode())) {
				return;
			}
			this.$propertyCreate(this.get_$m_CurrentNode());
			this.getProperty(ev);
			return;
		},
		$buttonDisabled: function(di) {
			if (ss.isNullOrUndefined(di)) {
				$control_UI.disable(this.get_setGetPropertyBtn(), true);
				$control_UI.disable(this.get_getPropertyBtn(), true);
				$control_UI.disable(this.get_setPropertyBtn(), true);
				return;
			}
			var ctrls = this.get_propertyList();
			$control_UI.disable(this.get_setGetPropertyBtn(), ss.isNullOrUndefined(ctrls));
			$control_UI.disable(this.get_getPropertyBtn(), ss.isNullOrUndefined(ctrls));
			$control_UI.disable(this.get_setPropertyBtn(), ss.isNullOrUndefined(ctrls));
		},
		$propertyCreate: function(di) {
			if (ss.isNullOrUndefined(di)) {
				return;
			}
			$(document).trigger($control_UIEventNames.propertyWriteStart);
			di.setUIDevice(this);
			this.get_propertyList().html('');
			this.get_propertyList().append(di.getPropertys());
			this.get_propertyList().listview().listview('refresh');
			this.get_propertyList().listview().trigger('create');
			$(document).trigger($control_UIEventNames.propertyWriteEnd);
		},
		$setInitParams: function(elem, e) {
			var macaddrfrm = new RegExp('([0-9A-Fa-f]{2}):([0-9A-Fa-f]{2}):([0-9A-Fa-f]{2}):([0-9A-Fa-f]{2}):([0-9A-Fa-f]{2}):([0-9A-Fa-f]{2})');
			var ipaddrfrm = new RegExp('([0-9]{1,3}).([0-9]{1,3}).([0-9]{1,3}).([0-9]{1,3})');
			var m = macaddrfrm.exec(this.get_macAddrTxt().val());
			if (ss.isNullOrUndefined(m)) {
				return;
			}
			var macAddr = '"macAddr":[' + parseInt(m[1], 16) + ',' + parseInt(m[2], 16) + ',' + parseInt(m[3], 16) + ',' + parseInt(m[4], 16) + ',' + parseInt(m[5], 16) + ',' + parseInt(m[6], 16) + ']';
			var dhcpEnable = '"dhcpEnable":' + this.get_dhcpEnableCb().prop('checked').toString();
			m = ipaddrfrm.exec(this.get_ipAddrTxt().val());
			if (ss.isNullOrUndefined(m)) {
				return;
			}
			var ipAddr = '"ipAddr":[' + parseInt(m[1]) + ',' + parseInt(m[2]) + ',' + parseInt(m[3]) + ',' + parseInt(m[4]) + ']';
			m = ipaddrfrm.exec(this.get_netmaskTxt().val());
			if (ss.isNullOrUndefined(m)) {
				return;
			}
			var netmask = '"netmask":[' + parseInt(m[1]) + ',' + parseInt(m[2]) + ',' + parseInt(m[3]) + ',' + parseInt(m[4]) + ']';
			m = ipaddrfrm.exec(this.get_gatewayTxt().val());
			if (ss.isNullOrUndefined(m)) {
				return;
			}
			var gateway = '"gateway":[' + parseInt(m[1]) + ',' + parseInt(m[2]) + ',' + parseInt(m[3]) + ',' + parseInt(m[4]) + ']';
			this.$m_WampClient.sendCall('{}', 'jp.toppers.ecnl.procedure.setInitParams', '[]', '{' + macAddr + ',' + dhcpEnable + ',' + ipAddr + ',' + netmask + ',' + gateway + '}', ss.mkdel(this, this.setInitParamsRes));
		},
		setInitParamsRes: function(args) {
			var cmd = ss.unbox(ss.cast(ss.cast(args, Array)[0], ss.Int32));
			if (cmd !== 50) {
				$('#popup_dialog_caption').text('エラー');
				$('#popup_dialog_title').text('エラーが発生しました');
				$('#popup_dialog_text').text('初期データの書き込みに失敗しました。');
				$('#popup_dialog').popup('open', { transition: 'slidedown' });
				return;
			}
			$('#popup_dialog_caption').text('初期データ');
			$('#popup_dialog_title').text('初期データを書き込みました');
			$('#popup_dialog_text').text('初期データの書き込みに成功しました。');
			$('#popup_dialog').popup('open', { transition: 'slidedown' });
		},
		$getInitParams: function(elem, e) {
			this.$m_WampClient.sendCall('{}', 'jp.toppers.ecnl.procedure.getInitParams', '[]', '{}', ss.mkdel(this, this.getInitParamsRes));
		},
		getInitParamsRes: function(args) {
			var cmd = ss.unbox(ss.cast(ss.cast(args, Array)[0], ss.Int32));
			if (cmd !== 50) {
				$('#popup_dialog_caption').text('エラー');
				$('#popup_dialog_title').text('エラーが発生しました');
				$('#popup_dialog_text').text('初期データの読み込みに失敗しました。');
				$('#popup_dialog').popup('open', { transition: 'slidedown' });
				return;
			}
			var param = ss.cast(args, Array)[4];
			var macaddr = ss.cast(param.macAddr, Array);
			this.get_macAddrTxt().val(ss.formatString('{0:X02}:{1:X02}:{2:X02}:{3:X02}:{4:X02}:{5:X02}', macaddr[0], macaddr[1], macaddr[2], macaddr[3], macaddr[4], macaddr[5]));
			var dhcpEnable = !!param.dhcpEnable;
			this.get_dhcpEnableCb().prop('checked', dhcpEnable).checkboxradio('refresh');
			var ipaddr = ss.cast(param.ipAddr, Array);
			this.get_ipAddrTxt().val(ss.formatString('{0}:{1}:{2}:{3}', ipaddr[0], ipaddr[1], ipaddr[2], ipaddr[3]));
			var netmask = ss.cast(param.netmask, Array);
			this.get_netmaskTxt().val(ss.formatString('{0}:{1}:{2}:{3}', netmask[0], netmask[1], netmask[2], netmask[3]));
			var gateway = ss.cast(param.gateway, Array);
			this.get_gatewayTxt().val(ss.formatString('{0}:{1}:{2}:{3}', gateway[0], gateway[1], gateway[2], gateway[3]));
		},
		get_setPropertyBtn: function() {
			return $('#bt_set_property', $CtrlUI.$page);
		},
		get_getPropertyBtn: function() {
			return $('#bt_get_property', $CtrlUI.$page);
		},
		get_setGetPropertyBtn: function() {
			return $('#bt_set_get_property', $CtrlUI.$page);
		},
		get_kadecotBtn: function() {
			return $('#bt_kadecot', $CtrlUI.$page);
		},
		get_connectionSwt: function() {
			return $('#fs_connection', $CtrlUI.$page);
		},
		get_propertyList: function() {
			return $('#lv_property_list', $CtrlUI.$page);
		},
		get_searchBtn: function() {
			return $('#bt_search', $CtrlUI.$page);
		},
		get_nodeList: function() {
			return $('#node_list', $CtrlUI.$page);
		},
		get_propertyGroupBox: function() {
			return $('#lv_property_list', $CtrlUI.$page).find('li');
		},
		get_setInitParamsBtn: function() {
			return $('#bt_set_init_params', $CtrlUI.$page);
		},
		get_getInitParamsBtn: function() {
			return $('#bt_get_init_params', $CtrlUI.$page);
		},
		get_macAddrTxt: function() {
			return $('#txt_mac_addr', $CtrlUI.$page);
		},
		get_dhcpEnableCb: function() {
			return $('#cb_dhcp_enable', $CtrlUI.$page);
		},
		get_ipAddrTxt: function() {
			return $('#txt_ip_addr', $CtrlUI.$page);
		},
		get_netmaskTxt: function() {
			return $('#txt_netmask', $CtrlUI.$page);
		},
		get_gatewayTxt: function() {
			return $('#txt_gateway', $CtrlUI.$page);
		},
		pinMode_Change: function(fieldset, pin) {
			var value = fieldset.checkdValue();
			var dic = ss.mkdict(['pin', pin, 'mode', value]);
			this.$m_WampClient.sendCall('{"deviceId":' + 1 + '}', 'com.sonycsl.kadecot.arduino.pinMode', '[]', JSON.stringify(dic), ss.mkdel(this, this.pinMode));
		},
		digital_Change: function(button, flipswitch, pin) {
			var msg = new $ctrlui_WampApiKadecotSet(1, '2', new Array(2));
			var dic = ss.mkdict(['pin', pin, 'value', flipswitch.get_value()]);
			this.$m_WampClient.sendCall('{"deviceId":' + 1 + '}', 'com.sonycsl.kadecot.arduino.digitalWrite', '[]', JSON.stringify(dic), ss.mkdel(this, this.digitalWrite));
		},
		analogWrite_Change: function(range, pin) {
			var msg = new $ctrlui_WampApiKadecotSet(1, '2', new Array(2));
			var dic = ss.mkdict(['pin', pin, 'value', range.get_value$1()]);
			this.$m_WampClient.sendCall('{"deviceId":' + 1 + '}', 'com.sonycsl.kadecot.arduino.analogWrite', '[]', JSON.stringify(dic), ss.mkdel(this, this.pinMode));
		},
		analogRead: function(range, pin) {
			var dic = ss.mkdict(['pin', pin]);
			this.$m_WampClient.sendCall('{"deviceId":' + 1 + '}', 'com.sonycsl.kadecot.arduino.analogRead', '[]', JSON.stringify(dic), ss.mkdel(this, function(args) {
				this.digitalRead$1(args, range);
			}));
		},
		pinMode: function(args) {
			var switchData = ss.unbox(ss.cast(ss.cast(args, Array)[0], ss.Int32));
			if (switchData !== 50) {
				return;
			}
			var doc = ss.cast(args, Array)[4];
			if (ss.isValue(this.$m_DeviceController)) {
				if (ss.isNullOrUndefined(this.get_propertyList())) {
					return;
				}
			}
		},
		digitalWrite: function(args) {
			var switchData = ss.unbox(ss.cast(ss.cast(args, Array)[0], ss.Int32));
			if (switchData !== 50) {
				return;
			}
			var doc = ss.cast(args, Array)[4];
			if (ss.isValue(this.$m_DeviceController)) {
				if (ss.isNullOrUndefined(this.get_propertyList())) {
					return;
				}
			}
		},
		digitalRead: function(button, flip, i) {
			var dic = ss.mkdict(['pin', i]);
			var str = this.$m_WampClient.sendCall('{"deviceId":' + 1 + '}', 'com.sonycsl.kadecot.arduino.digitalRead', '[]', JSON.stringify(dic), ss.mkdel(this, function(arg) {
				this.digitalRead$1(arg, flip);
			}));
			if (ss.isNullOrUndefined(str)) {
				button.toggleClass$1('ui-icon-refresh', 'ui-icon-alert');
			}
			else {
				button.toggleClass$1('ui-icon-alert', 'ui-icon-refresh');
			}
		},
		digitalRead$1: function(args, uiobject) {
			var switchData = ss.unbox(ss.cast(ss.cast(args, Array)[0], ss.Int32));
			if (switchData !== 50) {
				return;
			}
			var doc = ss.cast(args, Array)[4];
			if (ss.isValue(this.$m_DeviceController)) {
				var msg = new $ctrlui_WebApiGrsaguraMessage(doc);
				if (ss.referenceEquals(ss.getInstanceType(uiobject), $control_UIFlipSwitch)) {
					var flip = ss.cast(uiobject, $control_UIFlipSwitch);
					flip.set_value(msg.value);
				}
				else if (ss.referenceEquals(ss.getInstanceType(uiobject), $control_UIRangeInput)) {
					var range = ss.cast(uiobject, $control_UIRangeInput);
					range.set_value$1(parseInt(msg.value));
					range.object.slider('refresh');
				}
			}
		}
	});
	ss.initClass($ElementExtensions, $asm, {});
	ss.initClass($jQueryMobile, $asm, {});
	ss.initClass($LoadingOption, $asm, {});
	$LoadingOption.$ctor1.prototype = $LoadingOption.prototype;
	ss.initEnum($control_KeyPress, $asm, { delete$1: 0, backSpace: 8, minus: 45, dot: 46, slash: 47, _0: 48, _1: 49, _2: 50, _3: 51, _4: 52, _5: 53, _6: 54, _7: 55, _8: 56, _9: 57, a: 65, b: 66, c: 67, d: 68, e: 69, f: 70, a$1: 97, b$1: 98, c$1: 99, d$1: 100, e$1: 101, f$1: 102 });
	ss.initClass($control_UI, $asm, {});
	ss.initClass($control_UIObject, $asm, {
		get_element: function() {
			return this.object.get(0);
		},
		change: function() {
			this.object.change();
			return this;
		},
		change$1: function(eventHandler) {
			this.object.change(eventHandler);
		},
		keypress: function(eventHandler) {
			this.object.keypress(eventHandler);
		},
		toggle: function(showOrHide) {
			return this.object.toggle(showOrHide);
		},
		attribute$1: function(attributeName, value) {
			this.object.attr(attributeName, value);
		},
		removeAttr: function(attributeName) {
			this.object.removeAttr(attributeName);
		},
		getAttribute: function(attributeName) {
			return this.object.attr(attributeName);
		},
		find: function(selector) {
			return this.object.find(selector);
		},
		removeClass: function(className) {
			this.object.removeClass(className);
			return this;
		},
		toggleClass: function(className, add) {
			this.object.toggleClass(className, add);
			return this;
		},
		toggleClass$1: function(deleteClass, addClass) {
			$control_UI.toggleClass(this.object, deleteClass, addClass);
			return this;
		},
		one: function(eventName, eventHandler) {
			this.object.one(eventName, eventHandler);
			return this;
		},
		attribute: function(nameValueMap) {
			this.object.attr(nameValueMap);
			return this;
		},
		click$1: function(eventHandler) {
			this.object.click(eventHandler);
			return this;
		},
		click: function(eventHandler) {
			this.object.click(eventHandler);
			return this;
		},
		get_text: function() {
			return this.object.text();
		},
		set_text: function(value) {
			this.object.text(value);
		},
		get_id: function() {
			return this.object.attr('id');
		},
		set_id: function(value) {
			this.object.attr('id', value);
		},
		get_for: function() {
			return this.object.attr('for');
		},
		set_for: function(value) {
			this.object.attr('for', value);
		},
		get_name: function() {
			return this.object.attr('name');
		},
		set_name: function(value) {
			this.object.attr('name', value);
		},
		get_dataRole: function() {
			return this.object.attr('data-role');
		},
		set_dataRole: function(value) {
			this.object.attr('data-role', value);
		},
		get_dataType: function() {
			return this.object.attr('data-type');
		},
		set_dataType: function(value) {
			this.object.attr('data-type', value);
		},
		get_title: function() {
			return this.object.attr('title');
		},
		set_title: function(value) {
			this.object.attr('title', value);
		},
		get_value: function() {
			if (this.object.is('fieldset')) {
				return this.object.find("input[type='radio']:checked").val();
			}
			return this.object.val();
		},
		set_value: function(value) {
			if (this.object.is('fieldset')) {
				this.object.find('input[type="radio"]').each(function(j, elem) {
					if (ss.referenceEquals($(elem).val(), value)) {
						$(elem).prop('checked', true);
					}
					else {
						$(elem).prop('checked', false);
					}
				});
				this.object.find('input[type="radio"]').checkboxradio().checkboxradio('refresh');
			}
			else {
				this.object.val(value);
				if (this.get_dataRole() === 'flipswitch') {
					this.object.val(value).flipswitch('refresh');
				}
			}
		},
		get_maxlength: function() {
			return parseInt(this.getAttribute('maxlength'));
		},
		set_maxlength: function(value) {
			this.attribute$1('maxlength', value.toString());
		},
		get_min: function() {
			return parseInt(this.object.attr('min'));
		},
		set_min: function(value) {
			this.object.attr('min', value.toString());
		},
		get_max: function() {
			return parseInt(this.object.attr('max'));
		},
		set_max: function(value) {
			this.object.attr('max', value.toString());
		},
		get_tabIndex: function() {
			return parseInt(this.object.attr('tabindex'));
		},
		set_tabIndex: function(value) {
			this.object.attr('tabindex', value.toString());
		},
		get_type: function() {
			return this.object.attr('type');
		},
		set_type: function(value) {
			this.object.attr('type', value);
		},
		append: function(uiobject) {
			this.object.append(uiobject.object);
		},
		append$1: function(object) {
			return this.object.append(object);
		},
		disable: function(flag) {
			if (flag) {
				this.attribute$1('disabled', 'disabled');
			}
			else {
				this.removeAttr('disabled');
			}
			return this;
		},
		children: function() {
			return this.object.children();
		},
		addClass: function(className) {
			this.object.addClass(className);
			return this;
		},
		pointerEvents: function(flag) {
			if (!flag) {
				this.object.css('pointer-events', 'none');
			}
			return this;
		},
		hide: function() {
			this.object.hide();
			return this;
		},
		show: function() {
			this.object.show();
			return this;
		},
		get_class: function() {
			return this.$ecnlclass;
		},
		set_class: function(value) {
			this.$ecnlclass = value;
			this.addClass(this.$ecnlClassString(value));
		},
		$ecnlClassString: function(class1) {
			if (class1 === 0) {
				return 'ecnl_pink';
			}
			else if (class1 === 1) {
				return 'ecnl_violet';
			}
			else if (class1 === 2) {
				return 'ecnl_light_green';
			}
			else if (class1 === 3) {
				return 'ecnl_light_blue';
			}
			else {
				return '';
			}
		}
	});
	$control_UIObject.$ctor2.prototype = $control_UIObject.$ctor3.prototype = $control_UIObject.$ctor1.prototype = $control_UIObject.prototype;
	ss.initClass($control_UIAnchor, $asm, {
		get_href: function() {
			return this.getAttribute('href');
		},
		set_href: function(value) {
			this.attribute$1('href', value);
		},
		get_target: function() {
			return this.getAttribute('target');
		},
		set_target: function(value) {
			this.attribute$1('target', value);
		}
	}, $control_UIObject);
	$control_UIAnchor.$ctor1.prototype = $control_UIAnchor.$ctor2.prototype = $control_UIAnchor.prototype;
	ss.initClass($control_UIButton, $asm, {
		get_button: function() {
			return this.object.hasClass('ui-btn');
		},
		set_button: function(value) {
			this.object.toggleClass('ui-btn', value);
		},
		get_noText: function() {
			return this.object.hasClass('ui-btn-icon-notext');
		},
		set_noText: function(value) {
			this.object.toggleClass('ui-btn-icon-notext', value);
		},
		get_cornerAll: function() {
			return this.object.hasClass('ui-corner-all');
		},
		set_cornerAll: function(value) {
			this.object.toggleClass('ui-corner-all', value);
		},
		get_buttonIcon: function() {
			return this.$getIcon();
		},
		set_buttonIcon: function(value) {
			this.object.toggleClass(this.$getClass(value), true);
		},
		get_href: function() {
			return this.object.attr('href');
		},
		set_href: function(value) {
			this.object.attr('href', value);
		},
		$getClass: function(icon) {
			var text = '';
			if (icon === 0) {
				text = 'delete';
			}
			else if (icon === 1) {
				text = 'refresh';
			}
			else if (icon === 2) {
				text = 'back';
			}
			return 'ui-icon-' + text;
		},
		$getIcon: function() {
			if (this.object.hasClass('ui-icon-delete')) {
				return 0;
			}
			else if (this.object.hasClass('ui-icon-refresh')) {
				return 1;
			}
			else if (this.object.hasClass('ui-icon-back')) {
				return 2;
			}
			return 0;
		}
	}, $control_UIObject);
	ss.initClass($control_UIBackButton, $asm, {}, $control_UIButton);
	ss.initEnum($control_UIButton$Icon, $asm, { delete$1: 0, refresh: 1, back: 2 });
	ss.initClass($control_UITextInput, $asm, {
		get_dataClearBtn: function() {
			return /^\s*true\s*$/i.test(this.getAttribute('data-clear-btn'));
		},
		set_dataClearBtn: function(value) {
			if (value) {
				this.attribute$1('data-clear-btn', value.toString());
			}
			else {
				this.removeAttr('data-clear-btn');
			}
			;
		},
		get_placeholder: function() {
			return this.getAttribute('placeholder');
		},
		set_placeholder: function(value) {
			this.attribute$1('placeholder', value);
		},
		get_dataOptions: function() {
			return JSON.parse(this.getAttribute('data-options'));
		},
		set_dataOptions: function(value) {
			this.attribute$1('data-options', JSON.stringify(value));
		},
		keyup: function(eventHandler) {
			this.object.keyup(eventHandler);
			return this.object;
		},
		on: function(eventName, eventHandler) {
			this.object.on(eventName, eventHandler);
			return this.object;
		},
		get_readOnly: function() {
			return this.getAttribute('readonly') === 'readonly';
		},
		set_readOnly: function(value) {
			if (value) {
				this.attribute$1('readonly', 'readonly');
			}
			else {
				this.removeAttr('readonly');
			}
		}
	}, $control_UIObject);
	$control_UITextInput.$ctor1.prototype = $control_UITextInput.prototype;
	ss.initClass($control_UICalBox, $asm, {
		get_dY: function() {
			return this.$getCal(0);
		},
		get_dM: function() {
			return this.$getCal(1);
		},
		get_dD: function() {
			return this.$getCal(2);
		},
		get_xY: function() {
			return ss.formatNumber(this.get_dY(), 'X4');
		},
		get_xM: function() {
			return ss.formatNumber(this.get_dM(), 'X2');
		},
		get_xD: function() {
			return ss.formatNumber(this.get_dD(), 'X2');
		},
		get_YYYYMMDD: function() {
			return this.get_xY() + this.get_xM() + this.get_xD();
		},
		get_yyyymmdd: function() {
			return ss.formatNumber(this.get_dY(), 'D4') + '/' + ss.formatNumber(this.get_dM(), 'D2') + '/' + ss.formatNumber(this.get_dD(), 'D2');
		},
		$dateBoxCreate: function(ele, ev) {
			$control_UI.toggleClass($(ele).parents().find('.ui-icon-grid'), 'ui-icon-grid', 'ui-icon-calendar');
		},
		$getCal: function(i) {
			var x = 0;
			if (this.get_value().indexOf('/') !== -1) {
				var value = this.get_value().split('/');
				x = parseInt(value[i]);
			}
			return x;
		}
	}, $control_UITextInput);
	ss.initClass($control_UICalBox$UIInputDate, $asm, {}, $control_UIObject);
	ss.initClass($control_UICollapsibleset, $asm, {
		get_dataInset: function() {
			return /^\s*true\s*$/i.test(this.object.attr('data-inset'));
		},
		set_dataInset: function(value) {
			this.object.attr('data-inset', value.toString());
		}
	}, $control_UIObject);
	$control_UICollapsibleset.$ctor2.prototype = $control_UICollapsibleset.$ctor1.prototype = $control_UICollapsibleset.prototype;
	ss.initClass($control_UIDeviceController, $asm, {
		getUINodeList: function(listitem_click) {
			var nodelist = new $control_UINodeList(this.deviceController.get_nodeList(), listitem_click);
			return nodelist.getNodeList();
		}
	});
	ss.initClass($control_UIDeviceController$UIDeviceInfo, $asm, {
		uiDeviceSet: function(di, Page) {
			this.di = di;
			this.page = Page;
			if (ss.cast(di.get_data(), $ctrlui_WampApiKadecotDevice).protocol === 'arduino') {
				this.$elements = di.setPin(Page);
			}
			else {
				this.$elements = di.setEchonetlite(Page);
			}
		},
		getPropertys: function() {
			return $(this.$elements);
		},
		getData: function() {
			var $t1 = this.di.get_properties();
			for (var $t2 = 0; $t2 < $t1.length; $t2++) {
				var dpi = $t1[$t2];
				if (ss.isValue(dpi.get_data())) {
					dpi.getValue(dpi.get_data());
				}
			}
		},
		isWait: function() {
			var flag = false;
			if (ss.isValue(this.di) && ss.isValue(this.di.get_properties())) {
				var $t1 = this.di.get_properties();
				for (var $t2 = 0; $t2 < $t1.length; $t2++) {
					var dpi = $t1[$t2];
					if (ss.isValue(dpi) && dpi.get_isWaitRes()) {
						flag = true;
					}
				}
			}
			return flag;
		},
		setEchonetlite: function(ctrlui) {
			var ele = new Array(this.di.get_properties().length);
			var i = 0;
			var $t1 = this.di.get_properties();
			for (var $t2 = 0; $t2 < $t1.length; $t2++) {
				var dpi = $t1[$t2];
				dpi.setProperty(dpi);
				dpi.setName(this.di);
				dpi.setKadecotGet(ss.mkdel(this.page, this.page.kadecotGet$1));
				dpi.setKadecotSet(ss.mkdel(this.page, this.page.kadecotSet));
				ele[i++] = dpi.getElement();
			}
			return ele;
		},
		setPin: function(ctrlui) {
			this.page = ctrlui;
			var ele = new Array(20);
			var element;
			var j = 0;
			for (var i = 0; i <= 19; i++) {
				element = this.setPin$1(i, this.page);
				ele[j++] = element[0];
				ele[j++] = element[1];
			}
			return ele;
		},
		setPin$1: function(i, Page) {
			var uiPin = new $control_UIPin.$ctor1(i);
			uiPin.setPinModeChange(ss.mkdel(Page, Page.pinMode_Change));
			uiPin.setDigitalChange(ss.mkdel(Page, Page.digital_Change));
			uiPin.setReadButtonClick(ss.mkdel(Page, Page.digitalRead));
			uiPin.setAnalogWriteChange(ss.mkdel(Page, Page.analogWrite_Change));
			uiPin.setAnalogReadChange(ss.mkdel(Page, Page.analogRead));
			return uiPin.getElements();
		}
	});
	ss.initClass($control_UIDeviceController$UIDevicePropertyInfo, $asm, {
		getElement: function() {
			return this.$grpbox.object.get(0);
		},
		setName: function(di) {
			var nameid = di.getObjectID() + this.$dpi.getPropertyCode('X2');
			this.$grpbox.setName(nameid);
		},
		get_errorText: function() {
			return this.$grpbox.get_errorText();
		},
		set_errorText: function(value) {
			this.$grpbox.set_errorText(value);
		},
		getMaxLength: function() {
			var len = 2 * (ss.isNullOrUndefined(this.$dpi.get_propertyInfo()) ? this.$dpi.get_size() : (((this.$dpi.get_propertyInfo().arrayCount === 0) ? 1 : this.$dpi.get_propertyInfo().arrayCount) * this.$dpi.get_propertyInfo().size));
			return len;
		},
		setProperty: function(dpi) {
			this.$dpi = dpi;
			this.$grpbox = new $control_UIGrpBox.$ctor1(dpi);
		},
		setKadecotGet: function(KadecotGet) {
			var opeSt = null;
			//現在時刻設定
			if (this.$dpi.get_propertyCode() === 151) {
			}
			else if (this.$dpi.get_propertyCode() === 152) {
			}
			else if (this.$dpi.get_inputTypes().length === 0) {
			}
			else if (this.$dpi.get_propertyCode() === 128 && this.$dpi.get_inputTypes().length === 1 && this.$dpi.get_inputTypes()[0].mode === 1) {
			}
			else {
				var $t1 = this.$dpi.get_inputTypes();
				for (var $t2 = 0; $t2 < $t1.length; $t2++) {
					var pii = $t1[$t2];
					if (this.$dpi.get_propertyCode() === 128) {
						opeSt = this.$dpi;
					}
				}
				if (ss.isValue(opeSt)) {
					KadecotGet(opeSt);
				}
			}
		},
		setKadecotSet: function(KadecotSet) {
			this.$grpbox.kadecotSet = KadecotSet;
		},
		getValue: function(data) {
			this.$grpbox.setGetValue(data);
		}
	});
	ss.initClass($control_UIDiv, $asm, {}, $control_UIObject);
	ss.initClass($control_UIEventHandler, $asm, {});
	ss.initClass($control_UIEventNames, $asm, {});
	ss.initClass($control_UIFieldContain, $asm, {}, $control_UIObject);
	$control_UIFieldContain.$ctor1.prototype = $control_UIFieldContain.$ctor2.prototype = $control_UIFieldContain.prototype;
	ss.initClass($control_UIFieldSet, $asm, {
		checkdValue: function() {
			return $("input[type='radio']:checked", this.object).val();
		},
		append$2: function(radio) {
			if (ss.isNullOrUndefined(this.radioList)) {
				this.radioList = [];
			}
			this.radioList.push(new $control_UIFieldSet$RadioInput(radio.label, radio.radio));
			var last = Enumerable.from(this.radioList).last();
			this.append(last.label);
			this.append(last.radio);
		},
		get_controlGroup: function() {
			return this.$_ControlGroup;
		},
		set_controlGroup: function(value) {
			if (value) {
				this.set_dataRole('controlgroup');
			}
			else {
				this.set_dataRole('');
			}
			this.$_ControlGroup = value;
		},
		get_horizontal: function() {
			return this.$_Horizontal;
		},
		set_horizontal: function(value) {
			if (value) {
				this.set_dataType('horizontal');
			}
			else {
				this.set_dataType('');
			}
			this.$_Horizontal = value;
		}
	}, $control_UIObject);
	$control_UIFieldSet.$ctor1.prototype = $control_UIFieldSet.prototype;
	ss.initClass($control_UIFieldSet$RadioInput, $asm, {});
	ss.initClass($control_UIFlipSwitch, $asm, {}, $control_UIObject);
	$control_UIFlipSwitch.$ctor1.prototype = $control_UIFlipSwitch.prototype;
	ss.initClass($control_UIGridA, $asm, {}, $control_UIObject);
	ss.initClass($control_UIGridA$UIBlockA, $asm, {}, $control_UIObject);
	ss.initClass($control_UIGridA$UIBlockB, $asm, {}, $control_UIObject);
	ss.initClass($control_UIListviewItem, $asm, {}, $control_UIObject);
	$control_UIListviewItem.$ctor1.prototype = $control_UIListviewItem.prototype;
	ss.initClass($control_UIGrpBox, $asm, {
		setFlipSwitch: function() {
			var psi = ss.cast(this.$dpi.get_inputTypes()[0], $ctrlui_PropertySelectInput);
			// OFF
			var off = Enumerable.from(psi.option).first(function(kvp) {
				return kvp.key === 49;
			});
			// ON
			var on = Enumerable.from(psi.option).first(function(kvp1) {
				return kvp1.key === 48;
			});
			var OFF = new $control_UIOption.$ctor2(off.value, ss.formatNumber(off.key, 'X2'));
			var ON = new $control_UIOption.$ctor2(on.value, ss.formatNumber(on.key, 'X2'));
			var Flipswitch = new $control_UIFlipSwitch.$ctor1(OFF, ON);
			Flipswitch.change$1(ss.thisFix(ss.mkdel(this, this.$flip_Change)));
			Flipswitch.disable(!$control_UI.isRULE_SET(this.$dpi));
			this.fieldContain = new $control_UIFieldContain.$ctor2(this.$label, Flipswitch);
			//FieldContain.Object.Trigger("create");
			this.append(this.fieldContain);
		},
		setDefault: function() {
			var Textbox = new $control_UITextInput();
			Textbox.change$1(ss.thisFix(ss.mkdel(this, this.$text_Change)));
			var value = '';
			Textbox.set_value(value);
			this.fieldContain = new $control_UIFieldContain.$ctor2(this.$label, Textbox);
			this.append(this.fieldContain);
		},
		setRange: function(pii) {
			var range = new $control_UIRangeInput();
			range.change$1(ss.thisFix(ss.mkdel(this, this.$range_Change)));
			this.fieldContain = new $control_UIFieldContain.$ctor2(this.$label, range);
			var value = ss.cast(pii, $ctrlui_PropertyRangeInput).minimum;
			range.set_value$1(value);
			range.set_min(ss.cast(pii, $ctrlui_PropertyRangeInput).minimum);
			range.set_max(ss.cast(pii, $ctrlui_PropertyRangeInput).maximum);
			this.append$1(this.fieldContain.object).trigger('create');
		},
		$range_Change: function(elem, ev) {
			if (!this.checkValiable()) {
				return;
			}
			;
			if (!this.checkHex()) {
				return;
			}
			;
			var data = this.get_$byteValue();
			this.$dpi.set_inputData(data);
			this.kadecotSet(this.$dpi, data);
		},
		$setCalBox: function() {
			var Textbox = new $control_UICalBox();
			Textbox.change$1(ss.thisFix(ss.mkdel(this, this.$calbox_Change)));
			var epi = this.$dpi.get_propertyInfo();
			this.fieldContain = new $control_UIFieldContain.$ctor2(this.$label, Textbox);
			var len = this.$dpi.getMaxLength();
			var valueDescription = '';
			for (var $t1 = 0; $t1 < epi.fields.length; $t1++) {
				var jfi = epi.fields[$t1];
				valueDescription += '【' + jfi.description + ':' + jfi.valueDescription + '】';
			}
			Textbox.set_placeholder(this.$dpi.getText());
			Textbox.set_value('');
			//Textbox.Disable(!dpi.IsRULE_SET());
			Textbox.set_readOnly(!$control_UI.isRULE_SET(this.$dpi));
			Textbox.set_title(valueDescription);
			this.one('dateboxcreate', ss.thisFix(ss.mkdel(this, this.$calbox_Create)));
			this.append$1(this.fieldContain.object);
		},
		$calbox_Create: function(ele, ev) {
			var textinput = $(ev.target).parent('.ui-input-datebox');
			textinput.append(this.backButton.object);
			this.backButton.click$1(ss.thisFix(ss.mkdel(this, this.$backButtonClick)));
		},
		$calbox_Change: function(elem, ev) {
			var Calbox = ss.cast(this.fieldContain.input, $control_UICalBox);
			var data = new Array(4);
			this.removeClass$1();
			if (!(Calbox.get_dY() >= 1 && Calbox.get_dY() < 9999 && (Calbox.get_dM() >= 1 && Calbox.get_dM() <= 12 && (Calbox.get_dD() >= 1 && Calbox.get_dD() <= 31)))) {
				this.toggleClass('ecnl_pink', true);
			}
			else {
				data[0] = parseInt(Calbox.get_xY().substr(0, 2), 16);
				data[1] = parseInt(Calbox.get_xY().substr(2, 2), 16);
				data[2] = parseInt(Calbox.get_xM(), 16);
				data[3] = parseInt(Calbox.get_xD(), 16);
				this.$dpi.set_inputData(data);
				this.$dpi.set_isWaitRes(true);
				this.set_class(2);
			}
		},
		$flip_Change: function(elem, ev) {
			var data = this.get_$byteValue();
			this.$dpi.set_inputData(data);
			this.kadecotSet(this.$dpi, data);
		},
		$setTextInput: function() {
			var Textbox = new $control_UITextInput();
			Textbox.change$1(ss.thisFix(ss.mkdel(this, this.$text_Change)));
			Textbox.keypress(ss.thisFix(ss.mkdel(this, this.$text_KeyPress)));
			Textbox.keyup(ss.thisFix(ss.mkdel(this, this.$text_Keyup)));
			Textbox.on('textinputcreate', ss.thisFix(ss.mkdel(this, this.$text_Create)));
			var epi = this.$dpi.get_propertyInfo();
			var valueDescription = '';
			for (var $t1 = 0; $t1 < epi.fields.length; $t1++) {
				var jfi = epi.fields[$t1];
				valueDescription += '【' + jfi.description + ':' + jfi.valueDescription + '】';
			}
			Textbox.set_value('');
			//Textbox.Disable(!dpi.IsRULE_SET());
			Textbox.set_readOnly(!$control_UI.isRULE_SET(this.$dpi));
			Textbox.set_maxlength(this.$dpi.getMaxLength());
			Textbox.set_title(valueDescription);
			this.fieldContain = new $control_UIFieldContain.$ctor2(this.$label, Textbox);
			this.append$1(this.fieldContain.object);
		},
		$setTimebox: function() {
			var Textbox = new $control_UITimeBox();
			Textbox.change$1(ss.thisFix(ss.mkdel(this, this.$timebox_Change$1)));
			var epi = this.$dpi.get_propertyInfo();
			this.fieldContain = new $control_UIFieldContain.$ctor2(this.$label, Textbox);
			var len = this.$dpi.getMaxLength();
			var valueDescription = '';
			for (var $t1 = 0; $t1 < epi.fields.length; $t1++) {
				var jfi = epi.fields[$t1];
				valueDescription += '【' + jfi.description + ':' + jfi.valueDescription + '】';
			}
			Textbox.set_placeholder(this.$dpi.getText());
			Textbox.set_value('');
			Textbox.set_title(valueDescription);
			//Textbox.Disable(!dpi.IsRULE_SET());
			Textbox.set_readOnly(!$control_UI.isRULE_SET(this.$dpi));
			this.append(this.fieldContain);
			this.one('dateboxcreate', ss.thisFix(ss.mkdel(this, this.$timebox_Create)));
		},
		$setFieldSet: function(pii) {
			var FieldSet = new $control_UIFieldSet();
			var Label = new $control_UILabel();
			Label.set_text(this.$dpi.getText());
			FieldSet.set_horizontal(true);
			FieldSet.change$1(ss.thisFix(ss.mkdel(this, this.$select_Change)));
			this.fieldContain = new $control_UIFieldContain.$ctor2(Label, FieldSet);
			var $t1 = ss.cast(pii, $ctrlui_PropertySelectInput).option.getEnumerator();
			try {
				while ($t1.moveNext()) {
					var opt = $t1.current();
					var Radio = new $control_UIRadioInput.$ctor1(ss.formatNumber(opt.key, 'X'));
					FieldSet.append$2(new $control_UIFieldSet$RadioInput(new $control_UILabel.$ctor1(opt.value), Radio));
				}
			}
			finally {
				$t1.dispose();
			}
			this.append$1(this.fieldContain.object);
		},
		$select_Change: function(elem, ev) {
			var data = this.get_$byteValue();
			this.$dpi.set_inputData(data);
			this.kadecotSet(this.$dpi, data);
		},
		$timebox_Change$1: function(ele, ev) {
			this.removeClass$1();
			if (this.get_changed()) {
				this.$timebox_Change();
			}
		},
		$timebox_Change: function() {
			var value = this.fieldContain.input.get_value();
			var val = value.split(':');
			var data = new Array(2);
			data[0] = parseInt(val[0], 10);
			data[1] = parseInt(val[1], 10);
			var hex = ss.formatNumber(data[0], 'X2') + ss.formatNumber(data[1], 'X2');
			this.removeClass$1();
			this.$dpi.set_inputData(data);
			this.$dpi.set_isWaitRes(true);
			this.toggleClass('ecnl_light_green', true);
		},
		$timebox_Create: function(ele, ev) {
			var textinput = $(ev.target).parent('.ui-input-datebox');
			textinput.append(this.backButton.object);
			this.backButton.click$1(ss.thisFix(ss.mkdel(this, this.$backButtonClick)));
		},
		$text_Keyup: function(elem, ev) {
			$(elem).change();
		},
		$text_Create: function(elem, ev) {
			var textinput = $(ev.target).parent('.ui-input-text');
			textinput.append(this.backButton.object);
			this.backButton.click$1(ss.thisFix(ss.mkdel(this, this.$backButtonClick)));
		},
		$text_KeyPress: function(elem, ev) {
			var number = ev.which >= 48 && ev.which <= 57;
			var lower = ev.which >= 97 && ev.which <= 102;
			var upper = ev.which >= 65 && ev.which <= 70;
			var BackSpace = ev.which === 8;
			var Delete = ev.which === 0;
			var flag = !number && !lower && (!BackSpace && !Delete) && !upper;
			if (flag) {
				ev.preventDefault();
			}
		},
		$backButtonClick: function(elem, ev) {
			this.fieldContain.input.set_value(this.toText(this.getValue));
			this.fieldContain.input.object.change();
		},
		$text_Change: function(elem, ev) {
			if (this.get_changed()) {
				this.removeClass$1();
				if (!this.checkValiable()) {
					return;
				}
				;
				if (!this.checkHex()) {
					return;
				}
				;
				this.$setDpi();
			}
		},
		get_$inputType: function() {
			return ss.getInstanceType(this.fieldContain.input);
		},
		$toByteValue: function() {
			var Value = this.fieldContain.input.get_value();
			var data = new Array(ss.Int32.div(Value.length, 2));
			for (var i = 0, j = 0; j < Value.length; j += 2) {
				data[i++] = parseInt(Value.substr(j, 2), 16);
			}
			return data;
		},
		$setDpi: function() {
			this.$dpi.set_inputData(this.get_$byteValue());
			this.$dpi.set_isWaitRes(true);
			this.set_errorText('');
			this.toggleClass((this.$user ? 'ecnl_light_green' : 'ecnl_light_blue'), true);
		},
		get_$byteValue: function() {
			return this.$toByteValue();
		},
		get_errorText: function() {
			return this.$liAside.get_text();
		},
		set_errorText: function(value) {
			this.$liAside.set_text(value);
		},
		removeClass$1: function() {
			this.set_errorText('');
			this.backButton.hide();
			this.removeClass('ecnl_pink');
			this.removeClass('ecnl_violet');
			this.removeClass('ecnl_light_green');
			this.removeClass('ecnl_light_blue');
		},
		setName: function(nameid) {
			var name = nameid;
			if (ss.referenceEquals(this.get_$inputType(), $control_UICalBox)) {
				name = 'cal_' + nameid;
			}
			else if (ss.referenceEquals(this.get_$inputType(), $control_UITimeBox)) {
				name = 'tmb_' + nameid;
			}
			else if (ss.referenceEquals(this.get_$inputType(), $control_UITextInput)) {
				name = 'txt_' + nameid;
			}
			else if (ss.referenceEquals(this.get_$inputType(), $control_UIRadioInput)) {
				name = 'rng_' + nameid;
			}
			else if (ss.referenceEquals(this.get_$inputType(), $control_UIFlipSwitch)) {
				name = 'flp_' + nameid;
				var Flipswitch = ss.cast(this.fieldContain.input, $control_UIFlipSwitch);
				var val0 = Flipswitch.OFF.get_value();
				var val1 = Flipswitch.ON.get_value();
				Flipswitch.OFF.set_id(name + val0);
				Flipswitch.ON.set_id(name + val1);
			}
			else if (ss.referenceEquals(this.get_$inputType(), $control_UIFieldSet)) {
				name = 'sel_' + nameid;
				var FieldSet = ss.cast(this.fieldContain.input, $control_UIFieldSet);
				FieldSet.set_id(name);
				for (var i = 0; i < FieldSet.radioList.length; i++) {
					var id = name + FieldSet.radioList[i].radio.get_value();
					FieldSet.radioList[i].radio.set_name(name);
					FieldSet.radioList[i].radio.set_id(id);
					FieldSet.radioList[i].label.set_for(id);
				}
			}
			this.fieldContain.label.set_for(name);
			this.fieldContain.input.set_id(name);
			this.fieldContain.input.set_name(name);
		},
		checkHex: function() {
			var Value = this.fieldContain.input.get_value();
			if (!(new RegExp('^([0-9A-Fa-f][0-9A-Fa-f])+$')).test(Value)) {
				this.set_errorText('16進数で入力してください');
				this.toggleClass('ecnl_pink', true);
				return false;
			}
			return true;
		},
		checkValiable: function() {
			var epi = this.$dpi.get_propertyInfo();
			//可変長データの場合
			var valiable = this.$dpi.isValiable();
			var len = this.$dpi.getMaxLength();
			var Value = this.fieldContain.input.get_value();
			var length = Value.length !== len && (!valiable || Value.length > len);
			if (Value.length !== len && (!valiable || Value.length > len)) {
				this.set_errorText(len.toString() + '文字で入力してください');
				this.toggleClass((this.$user ? 'ecnl_pink' : 'ecnl_violet'), true);
				return false;
			}
			return true;
		},
		toTextDefault: function(data) {
			var value = '';
			if (ss.isNullOrUndefined(data)) {
				return '';
			}
			for (var i = 0; i < data.length; i++) {
				value += ss.formatNumber(data[i], 'X2');
			}
			return value;
		},
		toTextTimeBox: function(data) {
			var str = this.toTextDefault(data);
			if (str.length === 4) {
				var h = parseInt('0x' + str.substr(0, 2));
				var m = parseInt('0x' + str.substr(2, 2));
				return ss.formatNumber(h, 'D2') + ':' + ss.formatNumber(m, 'D2');
			}
			else {
				return '';
			}
		},
		toTextCal: function(data) {
			var str = this.toText(data);
			if (str.length === 8) {
				var y = parseInt('0x' + str.substr(0, 4));
				var m = parseInt('0x' + str.substr(4, 2));
				var d = parseInt('0x' + str.substr(6, 2));
				if (y < 1 || m < 1 || d < 1) {
					return ss.formatNumber(y, 'D4') + '/' + ss.formatNumber(m, 'D2') + '/' + ss.formatNumber(m, 'D2');
				}
				var date = new Date(y, m - 1, d);
				return ss.formatDate(date, 'yyyy/MM/dd');
			}
			else {
				return '';
			}
		},
		toText: function(data) {
			if (ss.isNullOrUndefined(data)) {
				return '';
			}
			if (ss.equals(ss.getInstanceType(this.fieldContain.input), $control_UITimeBox)) {
				return this.toTextTimeBox(data);
			}
			else if (ss.equals(ss.getInstanceType(this.fieldContain.input), $control_UICalBox)) {
				return this.toTextCal(data);
			}
			else {
				return this.toTextDefault(data);
			}
		},
		get_changed: function() {
			return !ss.equalsT(this.fieldContain.input.get_value(), this.toText(this.getValue));
		},
		grpBoxChange: function(ele, ev) {
			this.backButton.toggle(this.get_changed());
			if (!this.get_changed()) {
				this.$dpi.set_isWaitRes(false);
				this.$dpi.set_inputData(null);
				this.removeClass$1();
			}
		},
		setGetValue: function(data) {
			this.removeClass$1();
			this.getValue = data;
			this.fieldContain.input.set_value(this.toText(data));
			this.fieldContain.input.change();
		}
	}, $control_UIListviewItem);
	$control_UIGrpBox.$ctor1.prototype = $control_UIGrpBox.prototype;
	ss.initClass($control_UIH, $asm, {}, $control_UIObject);
	$control_UIH.$ctor1.prototype = $control_UIH.$ctor2.prototype = $control_UIH.prototype;
	ss.initClass($control_UIImg, $asm, {}, $control_UIObject);
	$control_UIImg.$ctor1.prototype = $control_UIImg.$ctor2.prototype = $control_UIImg.prototype;
	ss.initClass($control_UILabel, $asm, {}, $control_UIObject);
	$control_UILabel.$ctor1.prototype = $control_UILabel.$ctor3.prototype = $control_UILabel.$ctor4.prototype = $control_UILabel.$ctor2.prototype = $control_UILabel.prototype;
	ss.initClass($control_UILi, $asm, {}, $control_UIObject);
	ss.initClass($control_UIListDivider, $asm, {}, $control_UIListviewItem);
	$control_UIListDivider.$ctor1.prototype = $control_UIListDivider.$ctor2.prototype = $control_UIListDivider.$ctor3.prototype = $control_UIListDivider.prototype;
	ss.initClass($control_UIListview, $asm, {}, $control_UIObject);
	$control_UIListview.$ctor1.prototype = $control_UIListview.prototype;
	ss.initClass($control_UINodeList, $asm, {
		getNodeList: function() {
			return this.$info.children();
		},
		$listitem_Click: function(listitem, item) {
			//選択中クラスの追加
			$('.li-select').removeClass('li-select');
			listitem.addClass('li-select');
		}
	});
	ss.initEnum($control_UIObject$EcnlClass, $asm, { ecnl_pink: 0, ecnl_violet: 1, ecnl_light_green: 2, ecnl_light_blue: 3 });
	ss.initClass($control_UIOption, $asm, {}, $control_UIObject);
	$control_UIOption.$ctor2.prototype = $control_UIOption.$ctor1.prototype = $control_UIOption.prototype;
	ss.initClass($control_UIP, $asm, {}, $control_UIObject);
	$control_UIP.$ctor1.prototype = $control_UIP.$ctor2.prototype = $control_UIP.prototype;
	ss.initClass($control_UIPin, $asm, {
		$setUIPinMode: function(i) {
			this.uiPinMode.append(new $control_UILabel.$ctor4('INPUT', 'input' + i.toString(), 'pin' + i.toString()));
			this.uiPinMode.append(new $control_UIRadioInput.$ctor3('input' + i.toString(), 'pin' + i.toString(), 'INPUT'));
			this.uiPinMode.append(new $control_UILabel.$ctor4('OUTPUT', 'output' + i.toString(), 'pin' + i.toString()));
			this.uiPinMode.append(new $control_UIRadioInput.$ctor3('output' + i.toString(), 'pin' + i.toString(), 'OUTPUT'));
			this.uiPinMode.append(new $control_UILabel.$ctor4('INPUT_PULLUP', 'input_pullup' + i.toString(), 'pin' + i.toString()));
			this.uiPinMode.append(new $control_UIRadioInput.$ctor3('input_pullup' + i.toString(), 'pin' + i.toString(), 'INPUT_PULLUP'));
			return this.uiPinMode;
		},
		$setUIDigital: function(i) {
		},
		$setUIRange: function(i) {
			this.range = new $control_UIRangeInput.$ctor1(-1, 255);
			this.range.set_value$1(123);
		},
		$setAnalogRead: function(i) {
			this.analogRead = new $control_UIRangeInput.$ctor1(0, 1023);
			this.analogRead.handle = false;
		},
		getElements: function() {
			var elements = new Array(2);
			elements[0] = this.listDivider.get_element();
			elements[1] = this.get_element();
			return elements;
		},
		setPinModeChange: function(eventhandler) {
			if (ss.isValue(this.uiPinMode)) {
				this.uiPinMode.change$1(ss.thisFix(ss.mkdel(this, function(elem, ev) {
					eventhandler(this.uiPinMode, this.pin);
				})));
			}
		},
		setDigitalChange: function(eventhandler) {
			if (ss.isValue(this.flipSwitch)) {
				this.flipSwitch.change$1(ss.thisFix(ss.mkdel(this, function(elem, ev) {
					eventhandler(this.readButton, this.flipSwitch, this.pin);
				})));
			}
		},
		setReadButtonClick: function(eventhandler) {
			if (ss.isValue(this.flipSwitch)) {
				this.readButton.click$1(ss.thisFix(ss.mkdel(this, function(elem, ev) {
					eventhandler(this.readButton, this.flipSwitch, this.pin);
				})));
			}
		},
		setAnalogWriteChange: function(eventhandler) {
			if (ss.isValue(this.range)) {
				this.range.change$1(ss.thisFix(ss.mkdel(this, function(elem, ev) {
					eventhandler(this.range, this.pin);
				})));
			}
		},
		setAnalogReadChange: function(eventhandler) {
			if (ss.isValue(this.analogRead)) {
				this.readButton.click$1(ss.thisFix(ss.mkdel(this, function(elem, ev) {
					eventhandler(this.analogRead, this.pin);
				})));
			}
		}
	}, $control_UILi);
	$control_UIPin.$ctor1.prototype = $control_UIPin.prototype;
	ss.initClass($control_UIRadioInput, $asm, {}, $control_UIObject);
	$control_UIRadioInput.$ctor2.prototype = $control_UIRadioInput.$ctor3.prototype = $control_UIRadioInput.$ctor1.prototype = $control_UIRadioInput.prototype;
	ss.initClass($control_UIRangeInput, $asm, {
		$getMaxLength: function() {
			var min = 0;
			var j;
			if (this.get_min() < 0) {
				min = 1;
				j = -10;
				while (ss.Int32.div(this.get_max(), j) > 1) {
					min++;
					j *= 10;
				}
				min++;
			}
			var i = 1;
			j = 10;
			while (ss.Int32.div(this.get_max(), j) >= 1) {
				i++;
				j *= 10;
			}
			return ((i > min) ? i : min);
		},
		get_$dataHighlight: function() {
			return /^\s*true\s*$/i.test(this.getAttribute('data-highlight'));
		},
		set_$dataHighlight: function(value) {
			this.attribute$1('data-highlight', value.toString());
		},
		get_value$1: function() {
			if (this.get_value() === '') {
				this.set_value$1(this.get_min());
			}
			return parseInt(this.get_value());
		},
		set_value$1: function(value) {
			this.set_value(value.toString());
		},
		$sliderKeypress: function(ele, ev) {
			var number = ev.which >= 48 && ev.which <= 57;
			var BackSpace = ev.which === 8;
			var Delete = ev.which === 0;
			var minus = ev.which === 45;
			var flag = !number && !minus && (!BackSpace && !Delete);
			if (flag) {
				ev.preventDefault();
			}
		},
		$sliderCreate: function(ele, ev) {
			this.sliderHandle = new $control_UIRangeInput$UIHandle(ev);
			this.sliderHandle.toggle(this.handle);
			var SliderTrack = new $control_UIRangeInput$UISliderTrack(ev);
			SliderTrack.pointerEvents(this.handle);
			this.pointerEvents(this.handle);
		}
	}, $control_UIObject);
	$control_UIRangeInput.$ctor1.prototype = $control_UIRangeInput.prototype;
	ss.initClass($control_UIRangeInput$UIHandle, $asm, {}, $control_UIObject);
	ss.initClass($control_UIRangeInput$UISliderTrack, $asm, {}, $control_UIObject);
	ss.initClass($control_UITimeBox, $asm, {
		$dateBoxCreate: function(ele, ev) {
			$control_UI.toggleClass($(ele).parents().find('.ui-icon-grid'), 'ui-icon-grid', 'ui-icon-clock');
		}
	}, $control_UITextInput);
	ss.initEnum($ctrlui_$State, $asm, { $value: 0, $equalOrRange: 1, $description: 2, $unitOrCommaOrEnd: 3, $rengeMax: 4, $startParenthesis: 5, $rangeDescription: 6, $minDescription: 7, $rangeSeparetor: 8, $maxDescription: 9, $endParenthesis: 10, $commaOrEnd: 11, $bitFieldLSB: 12, $bitRange: 13, $bitFieldMSB: 14, $bitFieldDescription: 15, $bitRangeEnd: 16, $colon: 17 });
	ss.initClass($ctrlui_$TokenInfo, $asm, {
		toString: function() {
			return this.$token;
		},
		$toInt32: function() {
			switch (this.$type) {
				case 1: {
					return parseInt(this.$token.substring(2), 16);
				}
				case 2: {
					return parseInt(this.$token);
				}
				default: {
					return 0;
				}
			}
		},
		$toInt64: function(typeName) {
			var temp;
			switch (this.$type) {
				case 1: {
					temp = this.$token.substring(2);
					return parseInt(temp, 16);
				}
				case 2: {
					return parseInt(this.$token);
				}
				default: {
					return 0;
				}
			}
		}
	});
	ss.initEnum($ctrlui_$TokenType, $asm, { $separetor: 0, $hexValue: 1, $numeric: 2, $string: 3, $end: 4 });
	ss.initClass($ctrlui_$ValueSet, $asm, {});
	ss.initClass($ctrlui_Value, $asm, { get_disp: null, getInitialValue: null });
	ss.initClass($ctrlui_BitField, $asm, {
		get_least: function() {
			return this.$_Least;
		},
		get_most: function() {
			return this.$_Most;
		},
		get_disp: function() {
			return this.$_Disp;
		},
		get_values: function() {
			return this.$_Values;
		},
		get_ranges: function() {
			return this.$_Ranges;
		},
		get_initailValue: function() {
			return this.$_InitailValue;
		},
		getInitialValue: function() {
			if (ss.isNullOrUndefined(this.$_InitailValue)) {
				return '';
			}
			return this.$_InitailValue.getInitialValue();
		},
		$addValue: function(option) {
			if (ss.isNullOrUndefined(this.$_InitailValue)) {
				this.$_InitailValue = option;
			}
			this.$_Values.push(option);
		},
		$addRange: function(range) {
			if (ss.isNullOrUndefined(this.$_InitailValue)) {
				this.$_InitailValue = range;
			}
			this.$_Ranges.push(range);
		},
		$isInRange: function(val64) {
			for (var $t1 = 0; $t1 < this.$_Values.length; $t1++) {
				var val = this.$_Values[$t1];
				if (val.get_val() !== val64) {
					continue;
				}
				return true;
			}
			for (var $t2 = 0; $t2 < this.$_Ranges.length; $t2++) {
				var rng = this.$_Ranges[$t2];
				if (rng.get_min() > val64 || rng.get_max() < val64) {
					continue;
				}
				return true;
			}
			return false;
		},
		$getInRangeValue: function(val64) {
			for (var $t1 = 0; $t1 < this.$_Values.length; $t1++) {
				var val = this.$_Values[$t1];
				if (val.get_val() !== val64) {
					continue;
				}
				return val;
			}
			for (var $t2 = 0; $t2 < this.$_Ranges.length; $t2++) {
				var rng = this.$_Ranges[$t2];
				if (rng.get_min() > val64 || rng.get_max() < val64) {
					continue;
				}
				return new $ctrlui_InRangeValue(rng, val64);
			}
			return null;
		}
	}, $ctrlui_Value);
	ss.initClass($ctrlui_BitFiledsValue, $asm, {
		get_disp: function() {
			return '';
		},
		getInitialValue: function() {
			return this.$_Value.toString(16).toUpperCase();
		}
	}, $ctrlui_Value);
	ss.initClass($ctrlui_DeviceController, $asm, {
		get_nodeList: function() {
			return this.$m_NodeList;
		},
		recvResponse: function(response) {
			var ni = {};
			var di = {};
			this.$getDevice(response.sender, response.seoj, ni, di);
			if (ss.isNullOrUndefined(ni.$)) {
				return false;
			}
			if (ss.isNullOrUndefined(di.$)) {
				return false;
			}
			for (var $t1 = 0; $t1 < response.properties.length; $t1++) {
				var prop = response.properties[$t1];
				if (prop.edt.length > 0) {
					//プロパティ値を採用
					di.$.$setPropertyData(prop.epc, prop.$getEdt());
				}
				else {
					// プロパティ値書き込み応答の場合
					if (response.esv === 113) {
						// 入力値を採用
						di.$.$adoptInputData(prop.epc);
					}
				}
			}
			return true;
		},
		recvResponse$1: function(di, esv, epc, edt) {
			if (edt.length > 0) {
				// プロパティ値を設定
				di.$setPropertyData(epc, edt);
			}
			else {
				// プロパティ値書き込み応答の場合
				if (esv === 113) {
					// 入力値を採用
					di.$adoptInputData(epc);
				}
			}
		},
		$getDevice: function(nodid, eoj, nod, obj) {
			obj.$ = null;
			nod.$ = this.$findNode(nodid);
			if (ss.isValue(nod.$)) {
				// ノードプロファイルの場合
				if (eoj.get_x1() === 14 && eoj.get_x2() === 240) {
					obj.$ = nod.$.get_profile();
				}
				else {
					obj.$ = this.$findDevice(nod.$, eoj);
				}
			}
		},
		$findNode: function(enodid) {
			var ni = null;
			switch (enodid) {
				case 0:
				case 1: {
					ni = this.$m_NodeList[0];
					break;
				}
				default: {
					ni = Enumerable.from(this.$m_NodeList).firstOrDefault(function(p) {
						return p.get_nodeId() === enodid;
					}, ss.getDefaultValue($ctrlui_DeviceController$NodeInfo));
					break;
				}
			}
			return ni;
		},
		$findDevice: function(ni, eoj) {
			var $t1 = ni.get_devices();
			for (var $t2 = 0; $t2 < $t1.length; $t2++) {
				var di = $t1[$t2];
				if (di.get_classGroupCode() === eoj.get_x1() && di.get_classCode() === eoj.get_x2() && di.get_instanceCode() === eoj.get_x3()) {
					return di;
				}
			}
			return null;
		}
	}, $control_UIDeviceController);
	ss.initClass($ctrlui_DeviceController$DeviceInfo, $asm, {
		get_classInfo: function() {
			return this.$m_ClassInfo;
		},
		get_profile: function() {
			return this.$m_Profile;
		},
		get_classGroupCode: function() {
			return this.$m_ClassGroupCode;
		},
		get_classCode: function() {
			return this.$m_ClassCode;
		},
		get_instanceCode: function() {
			return this.$m_InstanceCode;
		},
		get_properties: function() {
			return this.$m_Properties;
		},
		get_eObjId: function() {
			return this.$m_EObjId;
		},
		get_data: function() {
			return this.$2$DataField;
		},
		set_data: function(value) {
			this.$2$DataField = value;
		},
		getText: function() {
			if (ss.isValue(this.$m_ClassInfo)) {
				return this.$m_ClassInfo.description;
			}
			else {
				return ss.formatString('{0:X2}{1:X2}{2:X2}', this.$m_ClassGroupCode, this.$m_ClassCode, this.$m_InstanceCode);
			}
		},
		$setPropertyData: function(epc, edt) {
			var dpi = Enumerable.from(this.$m_Properties).firstOrDefault(function(p) {
				return p.get_propertyCode() === epc;
			}, ss.getDefaultValue($ctrlui_DeviceController$DevicePropertyInfo));
			if (ss.isNullOrUndefined(dpi)) {
				return;
			}
			dpi.setPropertyData(edt);
		},
		$adoptInputData: function(epc) {
			var dpi = Enumerable.from(this.$m_Properties).firstOrDefault(function(p) {
				return p.get_propertyCode() === epc;
			}, ss.getDefaultValue($ctrlui_DeviceController$DevicePropertyInfo));
			if (ss.isNullOrUndefined(dpi)) {
				return;
			}
			dpi.adoptInputData();
		},
		getObjectID: function() {
			var text = ss.formatNumber(this.get_classGroupCode(), 'X2') + ss.formatNumber(this.get_classCode(), 'X2') + ss.formatNumber(this.get_instanceCode(), 'X2');
			return text;
		},
		setUIDevice: function(page) {
			this.uiDeviceSet(this, page);
		}
	}, $control_UIDeviceController$UIDeviceInfo);
	$ctrlui_DeviceController$DeviceInfo.$ctor2.prototype = $ctrlui_DeviceController$DeviceInfo.$ctor1.prototype = $ctrlui_DeviceController$DeviceInfo.prototype;
	ss.initClass($ctrlui_DeviceController$DevicePropertyInfo, $asm, {
		get_propertyInfo: function() {
			return this.$m_PropertyInfo;
		},
		get_propertyCode: function() {
			return this.$m_PropertyCode;
		},
		getPropertyCode: function(format) {
			return ss.formatNumber(this.get_propertyCode(), format);
		},
		isValiable: function() {
			return (ss.isNullOrUndefined(this.get_propertyInfo()) ? true : ss.contains(this.get_propertyInfo().access, 'VARIABLE'));
		},
		get_flag: function() {
			return this.$m_Flag;
		},
		get_size: function() {
			return this.$m_Size;
		},
		getText: function() {
			if (ss.isValue(this.$m_PropertyInfo)) {
				return this.$m_PropertyInfo.description;
			}
			else {
				return ss.formatString('{0:X2}', this.$m_PropertyCode);
			}
		},
		get_inputData: function() {
			return this.$2$InputDataField;
		},
		set_inputData: function(value) {
			this.$2$InputDataField = value;
		},
		get_data: function() {
			return this.$m_Data;
		},
		get_isWaitRes: function() {
			return this.$2$IsWaitResField;
		},
		set_isWaitRes: function(value) {
			this.$2$IsWaitResField = value;
		},
		get_inputTypes: function() {
			return this.$m_InputTypes;
		},
		setPropertyData: function(edt) {
			this.$m_Data = edt;
		},
		adoptInputData: function() {
			this.$m_Data = this.get_inputData();
			this.set_inputData(null);
		},
		prepareInputInfo: function() {
			if (ss.isValue(this.$m_InputTypes)) {
				return;
			}
			this.$m_InputTypes = [];
			var epi = this.$m_PropertyInfo;
			if (ss.isNullOrUndefined(epi) || !epi.primitive) {
				return;
			}
			var valueRange = $ctrlui_ValueRange.parse(epi.valueDescription, epi);
			if (!ss.isNullOrEmptyString(epi.initialValue)) {
				valueRange.set_initailValue(epi.initialValue);
			}
			if (valueRange.get_values().length > 0) {
				var pii;
				pii = new $ctrlui_PropertySelectInput();
				var $t1 = valueRange.get_values();
				for (var $t2 = 0; $t2 < $t1.length; $t2++) {
					var option = $t1[$t2];
					pii.option.add(option.get_val(), option.get_disp());
				}
				this.$m_InputTypes.push(pii);
			}
			var $t3 = valueRange.get_ranges();
			for (var $t4 = 0; $t4 < $t3.length; $t4++) {
				var range = $t3[$t4];
				var pii1;
				pii1 = new $ctrlui_PropertyRangeInput();
				pii1.minimum = range.get_min();
				pii1.maximum = range.get_max();
				pii1.minDescrption = range.get_minDisp();
				pii1.maxDescrption = range.get_maxDisp();
				this.$m_InputTypes.push(pii1);
			}
		}
	}, $control_UIDeviceController$UIDevicePropertyInfo);
	$ctrlui_DeviceController$DevicePropertyInfo.$ctor1.prototype = $ctrlui_DeviceController$DevicePropertyInfo.prototype;
	ss.initClass($ctrlui_DeviceController$NodeInfo, $asm, {
		get_nodeId: function() {
			return this.$m_NodeId;
		},
		get_profile: function() {
			return this.$m_Profile;
		},
		get_devices: function() {
			return this.$m_Devices;
		},
		get_data: function() {
			return this.$1$DataField;
		},
		set_data: function(value) {
			this.$1$DataField = value;
		}
	});
	ss.initEnum($ctrlui_ENOD_ID, $asm, { noT_MATCH_ID: -1, multicasT_ID: 0, locaL_ID: 1, apI_ID: 2, remotE_ID: 3, tablE_NUM: 100 });
	ss.initEnum($ctrlui_EPC_FLAG, $asm, { NONE: 0, rulE_SET: 1, rulE_GET: 2, rulE_ANNO: 4, ANNOUNCE: 64, VARIABLE: 128 });
	ss.initClass($ctrlui_Information, $asm, {
		update: function(ele, id) {
			var page = $CtrlUI.getPage(ele);
			this.$info = $('#' + id, page);
			if (ss.isNullOrUndefined(this.$info)) {
				return;
			}
			$.ajax('./' + id + '.json').success(ss.mkdel(this, this.$success)).error(ss.mkdel(this, this.$error));
		},
		$success: function(data, textStatus, request) {
			var items = ss.cast((ss.referenceEquals(ss.getInstanceType(data), String) ? $.parseJSON(ss.cast(data, String)) : data), Array);
			var listview = (new $control_UIListview.$ctor1(['data-inset', true, 'data-theme', 'b'])).object;
			listview.append((new $control_UIListDivider.$ctor1('ソフトウェア情報')).object);
			for (var $t1 = 0; $t1 < items.length; $t1++) {
				var item = items[$t1];
				var title = ss.cast(item.title, String);
				var version = ss.cast(item.version, String);
				var link = ss.cast(item.link, String);
				var note = ss.cast(item.note, String);
				var a;
				if (ss.isNullOrEmptyString(link)) {
					a = new $control_UIAnchor.$ctor2(new $control_UIH.$ctor2(2, title));
					a.set_href('#');
				}
				else {
					a = new $control_UIAnchor.$ctor2(new $control_UIH.$ctor2(2, title));
					a.set_href(link);
					a.set_target('_blank');
				}
				a.append(new $control_UIP.$ctor1('Version ' + version));
				if (!ss.isNullOrEmptyString(note)) {
					a.append(new $control_UIP.$ctor1(note));
				}
				listview.append((new $control_UIListviewItem.$ctor1(a)).object);
			}
			var anchor = new $control_UIAnchor();
			anchor.set_target('_blank');
			anchor.set_href('http://www.core-s.co.jp');
			anchor.append$1((new $control_UIImg.$ctor2(['src', 'imgs/core-s.svg'])).object).append('コアーズ株式会社');
			var listitem = (new $control_UIListDivider.$ctor2(anchor)).object;
			listview.append(listitem);
			this.$info.html('');
			this.$info.append(listview);
			$('ul', this.$info).listview().listview('refresh');
			this.$info.popup('reposition', { positionTo: 'origin' });
		},
		$error: function(request, textStatus, error) {
			var listview = (new $control_UIListview.$ctor1(['data-inset', 'true', 'data-theme', 'b'])).object;
			listview.append((new $control_UIListDivider.$ctor1('ソフトウェア情報')).object);
			listview.append((new $control_UIListviewItem.$ctor1(new $control_UIP.$ctor1('通信エラー'))).object);
			this.$info.append(listview);
			$('ul', this.$info).listview().listview('refresh');
			this.$info.popup('reposition', { positionTo: 'origin' });
		}
	});
	ss.initClass($ctrlui_InRangeValue, $asm, {
		get_range: function() {
			return this.$_Range;
		},
		get_value: function() {
			return this.$_Value;
		},
		get_disp: function() {
			return this.$_Range.getDisp(this.$_Value);
		},
		getInitialValue: function() {
			return this.$_Value.toString(16).toUpperCase();
		}
	}, $ctrlui_Value);
	ss.initClass($ctrlui_JsonClassGroupInfo, $asm, {});
	ss.initClass($ctrlui_JsonClassInfo, $asm, {});
	ss.initClass($ctrlui_JsonFieldInfo, $asm, {});
	ss.initClass($ctrlui_JsonObjectInfo, $asm, {
		findProperty: function(epc) {
			for (var $t1 = 0; $t1 < this.properties.length; $t1++) {
				var item = this.properties[$t1];
				if (item.propertyCode === epc) {
					return item;
				}
			}
			return null;
		}
	});
	ss.initClass($ctrlui_JsonNodeInfo, $asm, {}, $ctrlui_JsonObjectInfo);
	ss.initClass($ctrlui_JsonPropertyInfo, $asm, {}, $ctrlui_JsonFieldInfo);
	$ctrlui_JsonPropertyInfo.$ctor1.prototype = $ctrlui_JsonPropertyInfo.prototype;
	ss.initClass($ctrlui_JsonSkeltonResult, $asm, {});
	ss.initClass($ctrlui_Option, $asm, {
		get_val: function() {
			return this.$_Val;
		},
		get_disp: function() {
			return this.$_Disp;
		},
		getInitialValue: function() {
			return this.$_Val.toString(16).toUpperCase();
		}
	}, $ctrlui_Value);
	ss.initClass($ctrlui_PropertyInputInfo, $asm, {});
	ss.initEnum($ctrlui_PropertyInputMode, $asm, { unknown: 0, select: 1, range: 2 });
	ss.initClass($ctrlui_PropertyRangeInput, $asm, {}, $ctrlui_PropertyInputInfo);
	ss.initClass($ctrlui_PropertySelectInput, $asm, {}, $ctrlui_PropertyInputInfo);
	ss.initClass($ctrlui_Range, $asm, {
		get_min: function() {
			return this.$_Min;
		},
		get_max: function() {
			return this.$_Max;
		},
		get_disp: function() {
			return this.$_Disp;
		},
		get_minDisp: function() {
			return this.$_MinDisp;
		},
		get_maxDisp: function() {
			return this.$_MaxDisp;
		},
		get_unit: function() {
			return this.$_Unit;
		},
		get_initialValue: function() {
			return this.$_InitialValue;
		},
		set_initialValue: function(value) {
			this.$_InitialValue = value;
		},
		getInitialValue: function() {
			return this.$_InitialValue.toString(16).toUpperCase();
		},
		getDisp: function(value) {
			return this.$_Disp + value.toString() + this.$_Unit;
		}
	}, $ctrlui_Value);
	$ctrlui_Range.$ctor1.prototype = $ctrlui_Range.prototype;
	ss.initClass($ctrlui_T_ECN_EOJ, $asm, {
		get_x1: function() {
			return this.$_eoj[0];
		},
		set_x1: function(value) {
			this.$_eoj[0] = value;
		},
		get_x2: function() {
			return this.$_eoj[1];
		},
		set_x2: function(value) {
			this.$_eoj[1] = value;
		},
		get_x3: function() {
			return this.$_eoj[2];
		},
		set_x3: function(value) {
			this.$_eoj[2] = value;
		}
	});
	ss.initClass($ctrlui_ValueRange, $asm, {
		get_values: function() {
			return this.$_Values;
		},
		get_ranges: function() {
			return this.$_Ranges;
		},
		get_bitFields: function() {
			return this.$_BitFields;
		},
		get_memberFields: function() {
			return this.$_MemberFields;
		},
		get_initailValue: function() {
			if (ss.isNullOrUndefined(this.$_InitailValue)) {
				return '';
			}
			return this.$_InitailValue.getInitialValue();
		},
		set_initailValue: function(value) {
			var val64 = parseInt(value);
			for (var $t1 = 0; $t1 < this.$_Values.length; $t1++) {
				var val = this.$_Values[$t1];
				if (val.get_val() !== val64) {
					continue;
				}
				this.$_InitailValue = val;
				return;
			}
			for (var $t2 = 0; $t2 < this.$_Ranges.length; $t2++) {
				var rng = this.$_Ranges[$t2];
				if (rng.get_min() > val64 || rng.get_max() < val64) {
					continue;
				}
				this.$_InitailValue = new $ctrlui_InRangeValue(rng, val64);
				return;
			}
			if (this.$_BitFields.length > 0) {
				var ok = true;
				var bitFlds = [];
				for (var $t3 = 0; $t3 < this.$_BitFields.length; $t3++) {
					var bit = this.$_BitFields[$t3];
					if (!bit.$isInRange(val64)) {
						ok = false;
						break;
					}
					bitFlds.push(bit.$getInRangeValue(val64));
				}
				if (ok) {
					this.$_InitailValue = new $ctrlui_BitFiledsValue(Array.prototype.slice.call(this.$_BitFields), Array.prototype.slice.call(bitFlds), val64);
					return;
				}
			}
		}
	});
	ss.initClass($ctrlui_WampApiKadecotDevice, $asm, {});
	ss.initClass($ctrlui_WampApiKadecotGet, $asm, {
		$getOption: function() {
			return '{"deviceId":' + this.deviceId.toString() + '}';
		},
		$getArguments: function() {
			return '{"propertyName":"' + this.propertyName + '"}';
		}
	});
	ss.initClass($ctrlui_WampApiKadecotRes, $asm, {});
	ss.initClass($ctrlui_WampApiKadecotSet, $asm, {
		$getOption: function() {
			return '{"deviceId":' + this.deviceId.toString() + '}';
		},
		$getArguments: function() {
			return '{"propertyName":"' + this.propertyName + '","propertyValue":' + JSON.stringify(this.propertyValue) + '}';
		}
	});
	ss.initClass($ctrlui_WebApiEchonetMessage, $asm, {});
	ss.initClass($ctrlui_WebApiGet, $asm, {
		addEpc: function(epc) {
			this.properties.push(new $ctrlui_WebApiPropertyCode(epc));
		},
		getJson: function() {
			return '{"esv_get":' + JSON.stringify(this) + '}';
		},
		getOption: function() {
			return '{"deviceId":' + this.deojid.toString() + '}';
		},
		getArguments: function() {
			return JSON.stringify(this);
		}
	});
	$ctrlui_WebApiGet.$ctor1.prototype = $ctrlui_WebApiGet.prototype;
	ss.initClass($ctrlui_WebApiGrsaguraMessage, $asm, {});
	ss.initClass($ctrlui_WebApiObjectInfo, $asm, {});
	ss.initClass($ctrlui_WebApiPropertyCode, $asm, {});
	$ctrlui_WebApiPropertyCode.$ctor1.prototype = $ctrlui_WebApiPropertyCode.prototype;
	ss.initClass($ctrlui_WebApiPropertyData, $asm, {
		$setEdt: function(_edt) {
			var sb = new ss.StringBuilder();
			for (var $t1 = 0; $t1 < _edt.length; $t1++) {
				var b = _edt[$t1];
				sb.append(ss.formatNumber(b, 'X2'));
			}
			return sb.toString();
		},
		$getEdt: function() {
			var result = new Array(ss.Int32.div(this.edt.length, 2));
			for (var i = 0, j = 0; j < this.edt.length; i++, j += 2) {
				result[i] = parseInt(this.edt.substr(j, 2), 16);
			}
			return result;
		}
	});
	$ctrlui_WebApiPropertyData.$ctor1.prototype = $ctrlui_WebApiPropertyData.prototype;
	ss.initClass($ctrlui_WebApiPropertyInfo, $asm, {});
	ss.initClass($ctrlui_WebApiSet, $asm, {
		addEdt: function(epc, edt) {
			this.properties.push(new $ctrlui_WebApiPropertyData.$ctor1(epc, edt));
		},
		getJson: function() {
			return '{"esv_set":' + JSON.stringify(this) + '}';
		},
		getOption: function() {
			return '{"deviceId":' + this.deojid.toString() + '}';
		},
		getArguments: function() {
			return JSON.stringify(this);
		}
	});
	$ctrlui_WebApiSet.$ctor1.prototype = $ctrlui_WebApiSet.prototype;
	ss.initClass($ctrlui_WebApiSetGet, $asm, {
		addEdt: function(epc, edt) {
			this.setProperties.push(new $ctrlui_WebApiPropertyData.$ctor1(epc, edt));
		},
		addEpc: function(epc) {
			this.getProperties.push(new $ctrlui_WebApiPropertyCode(epc));
		},
		getJson: function() {
			return '{"esv_set_get":' + JSON.stringify(this) + '}';
		},
		getOption: function() {
			return '{"deviceId":' + this.deojid.toString() + '}';
		},
		getArguments: function() {
			return JSON.stringify(this);
		}
	});
	$ctrlui_WebApiSetGet.$ctor1.prototype = $ctrlui_WebApiSetGet.prototype;
	ss.initEnum($Kadecot_WAMP_MSG_TYPE, $asm, { HELLO: 1, WELCOME: 2, ABORT: 3, CHALLENGE: 4, AUTHENTICATE: 5, GOODBYE: 6, HEARTBEAT: 7, ERROR: 8, PUBLISH: 16, PUBLISHED: 17, SUBSCRIBE: 32, SUBSCRIBED: 33, UNSUBSCRIBE: 34, UNSUBSCRIBED: 35, EVENT: 36, CALL: 48, CANCEL: 49, RESULT: 50, REGISTER: 64, REGISTERED: 65, UNREGISTER: 66, UNREGISTERED: 67, INVOCATION: 68, INTERRUPT: 69, YIELD: 70 });
	ss.initClass($Kadecot_WampClient, $asm, {
		connect: function(serverIp) {
			if (this.$on_open_flag) {
				return;
			}
			if (!ss.isNullOrEmptyString(serverIp)) {
				this.$url = serverIp;
				//port : 41314
			}
			else {
				this.$url = 'ws://localhost:8080/';
			}
			this.$webSocket = new WebSocket(this.$url, this.$plotocols);
			//webSocket.Origin 	 = origin;
			this.$webSocket.onopen = ss.delegateCombine(this.$webSocket.onopen, ss.mkdel(this, this.onOpen));
			this.$webSocket.onmessage = ss.delegateCombine(this.$webSocket.onmessage, ss.mkdel(this, this.onMessage));
			this.$webSocket.onclose = ss.delegateCombine(this.$webSocket.onclose, ss.mkdel(this, this.onClose));
			this.$webSocket.onerror = ss.delegateCombine(this.$webSocket.onerror, ss.mkdel(this, this.onError));
			//webSocket.Connect();
		},
		disconnect: function() {
			this.$webSocket.close();
		},
		addOpenCallback: function(callback) {
			this.$wampClientCore.set_openCallback(callback);
		},
		deleteOpenCallback: function() {
			this.$wampClientCore.deleteOpenCallback();
		},
		addCloseCallback: function(callback) {
			this.$wampClientCore.set_closeCallback(callback);
		},
		deleteCloseCallback: function() {
			this.$wampClientCore.deleteCloseCallback();
		},
		addGoodbyeCallback: function(callback) {
			this.$wampClientCore.set_goodbyeCallback(callback);
		},
		deleteGoodbyeCallback: function() {
			this.$wampClientCore.deleteGoodbyeCallback();
		},
		addHeartbeatCallback: function(callback) {
			this.$wampClientCore.set_heartbeatCallback(callback);
		},
		deleteHeartbeatCallback: function() {
			this.$wampClientCore.deleteHeartbeatCallback();
		},
		addInvocationCallback: function(request, callback) {
			this.$wampClientCore.get_invocationCallback().set_item(request, callback);
		},
		deleteInvocationCallback: function(request) {
			this.$wampClientCore.deleteInvocationCallback(request);
		},
		sendHello: function(realm, details, callback) {
			//serialize
			var data;
			data = $Kadecot_WampSerializer.createHello(realm, details);
			//encrypt data
			//set callback
			this.$wampClientCore.set_helloCallback(callback);
			//send data
			if (this.$on_open_flag === true) {
				this.$webSocket.send(data);
			}
			else {
				return null;
			}
			return data;
		},
		sendAuthenticate: function(signature, extra, callback) {
			//serialize
			var data;
			data = $Kadecot_WampSerializer.createAuthenticate(signature, extra);
			//encrypt data
			//set callback
			this.$wampClientCore.set_authenticateCallback(callback);
			//send data
			if (this.$on_open_flag) {
				this.$webSocket.send(data);
			}
			else {
				return null;
			}
			return data;
		},
		sendGoodbye: function(details, reason, callback) {
			//serialize
			var data;
			data = $Kadecot_WampSerializer.createGoodbye(details, reason);
			//encrypt data
			//set callback
			this.$wampClientCore.set_goodbyeCallback(callback);
			//send data
			if (this.$on_open_flag) {
				this.$webSocket.send(data);
			}
			else {
				return null;
			}
			return data;
		},
		sendHeartbeat: function(incomingSeq, outgoingSeq, discard) {
			//serialize
			var data;
			data = $Kadecot_WampSerializer.createHeartbeat(incomingSeq, outgoingSeq, discard);
			//encrypt data
			//send data
			if (this.$on_open_flag) {
				this.$webSocket.send(data);
			}
			else {
				return null;
			}
			return data;
		},
		sendError: function(type, request, details, error, arguments1, argumentsKw) {
			//serialize
			var data;
			data = $Kadecot_WampSerializer.createError(type, request, details, error, arguments1, argumentsKw);
			//encrypt data
			//send data
			if (this.$on_open_flag) {
				this.$webSocket.send(data);
			}
			else {
				return null;
			}
			return data;
		},
		sendPublish: function(options, topic, arguments1, argumentsKw, callback) {
			//serialize
			var data;
			var request;
			request = this.$request_id;
			this.$request_id++;
			data = $Kadecot_WampSerializer.createPublish(request, options, topic, arguments1, argumentsKw);
			//encrypt data
			//set callback
			this.$wampClientCore.get_publishCallback().set_item(request, callback);
			//send data
			if (this.$on_open_flag) {
				this.$webSocket.send(data);
			}
			else {
				return null;
			}
			return data;
		},
		sendSubscribe: function(options, topic, eventfunc, callback) {
			//serialize
			var data;
			var request;
			request = this.$request_id;
			this.$request_id++;
			data = $Kadecot_WampSerializer.createSubscribe(request, options, topic);
			//encrypt data
			//set callback
			this.$wampClientCore.get_subscribeCallback().set_item(request, callback);
			//set event callback
			this.$wampClientCore.get_eventCallback().set_item(request, eventfunc);
			//send data
			if (this.$on_open_flag) {
				this.$webSocket.send(data);
			}
			else {
				return null;
			}
			return data;
		},
		sendUnsubscribe: function(subscription, callback) {
			//serialize
			var data;
			var request;
			request = this.$request_id;
			this.$request_id++;
			data = $Kadecot_WampSerializer.createUnsubscribe(request, subscription);
			//encrypt data
			//set callback
			this.$wampClientCore.get_unsubscribeCallback().set_item(request, callback);
			//set Unsubscribe request id - subscription id
			this.$subscription_id_list.set_item(request, subscription);
			//send data
			if (this.$on_open_flag) {
				this.$webSocket.send(data);
			}
			else {
				return null;
			}
			return data;
		},
		sendCall: function(options, procedure, arguments1, argumentsKw, callback) {
			//serialize
			var data;
			var request;
			request = this.$request_id;
			this.$request_id++;
			data = $Kadecot_WampSerializer.createCall(request, options, procedure, arguments1, argumentsKw);
			//encrypt data
			//set callback
			this.$wampClientCore.get_callCallback().set_item(request, callback);
			//send data
			if (this.$on_open_flag === true) {
				this.$webSocket.send(data);
			}
			else {
				return null;
			}
			return data;
		},
		sendCancel: function(request, options, callback) {
			//serialize
			var data;
			data = $Kadecot_WampSerializer.createCancel(request, options);
			//encrypt data
			//set callback
			this.$wampClientCore.get_cancelCallback().set_item(request, callback);
			//send data
			if (this.$on_open_flag) {
				this.$webSocket.send(data);
			}
			else {
				return null;
			}
			return data;
		},
		sendRegister: function(options, procedure, callback) {
			//serialize
			var data;
			var request;
			request = this.$request_id;
			this.$request_id++;
			data = $Kadecot_WampSerializer.createRegister(request, options, procedure);
			//encrypt data
			//set callback
			this.$wampClientCore.get_registerCallback().set_item(request, callback);
			//send data
			if (this.$on_open_flag) {
				this.$webSocket.send(data);
			}
			else {
				return null;
			}
			return data;
		},
		sendUnregister: function(registration, callback) {
			//serialize
			var data;
			var request;
			request = this.$request_id;
			this.$request_id++;
			data = $Kadecot_WampSerializer.createUnregister(request, registration);
			//encrypt data
			//set callback
			this.$wampClientCore.get_unregisterCallback().set_item(request, callback);
			//send data
			if (this.$on_open_flag) {
				this.$webSocket.send(data);
			}
			else {
				return null;
			}
			return data;
		},
		sendYield: function(request, options, arguments1, argumentsKw) {
			//serialize
			var data;
			data = $Kadecot_WampSerializer.createYield(request, options, arguments1, argumentsKw);
			//encrypt data
			//send data
			if (this.$on_open_flag) {
				this.$webSocket.send(data);
			}
			return data;
		},
		onOpen: function(e) {
			this.$on_open_flag = true;
			var callback = this.$wampClientCore.get_openCallback();
			if (!ss.staticEquals(callback, null)) {
				callback();
			}
		},
		onClose: function(e) {
			this.$on_open_flag = false;
			//			webSocket = null;
			var callback = this.$wampClientCore.get_closeCallback();
			if (!ss.staticEquals(callback, null)) {
				callback();
			}
		},
		onError: function(e) {
			var eea = e;
			if (!ss.staticEquals(this.$errorCallback, null)) {
				this.$errorCallback(ss.cast(eea.message, String));
			}
		},
		onMessage: function(e) {
			var msg = e;
			//System.Diagnostics.Debug.WriteLine("[wamp perser] msg :" + msg.ToString());
			var data;
			var callback = null;
			//decrypt data
			//deserialize
			if (!!ss.isValue(msg.data)) {
				data = $Kadecot_WampDeserializer.parse(ss.cast(msg.data, String));
			}
			else {
				data = $Kadecot_WampDeserializer.parse(ss.cast(msg, String));
			}
			if (ss.isNullOrUndefined(data)) {
				return;
			}
			var list = ss.cast(data, Array);
			var switchData = ss.unbox(ss.cast(list[0], ss.Int32));
			switch (switchData) {
				case 2: {
					// WAMP SPEC: [WELCOME, Session|id, Details|dict]
					if (this.$challnge_flag) {
						this.$challnge_flag = false;
						callback = this.$wampClientCore.get_authenticateCallback();
						if (!ss.staticEquals(callback, null)) {
							callback(data);
						}
						else {
						}
					}
					else {
						callback = this.$wampClientCore.get_helloCallback();
						if (!ss.staticEquals(callback, null)) {
							callback(data);
						}
						else {
						}
					}
					break;
				}
				case 3: {
					// WAMP SPEC: [ABORT, Details|dict, Reason|uri]
					if (this.$challnge_flag) {
						this.$challnge_flag = false;
						callback = this.$wampClientCore.get_authenticateCallback();
						if (!ss.staticEquals(callback, null)) {
							callback(data);
						}
						else {
						}
					}
					else {
						callback = this.$wampClientCore.get_helloCallback();
						if (!ss.staticEquals(callback, null)) {
							callback(data);
						}
						else {
						}
					}
					break;
				}
				case 4: {
					// advanced WAMP SPEC: [CHALLENGE, AuthMethod|string, Extra|dict]
					this.$challnge_flag = true;
					callback = this.$wampClientCore.get_helloCallback();
					if (!ss.staticEquals(callback, null)) {
						callback(data);
					}
					else {
					}
					break;
				}
				case 6: {
					// WAMP SPEC: [GOODBYE, Details|dict, Reason|uri]
					callback = this.$wampClientCore.get_goodbyeCallback();
					if (!ss.staticEquals(callback, null)) {
						callback(data);
					}
					else {
					}
					break;
				}
				case 7: {
					// advanced WAMP SPEC: [AUTHENTICATE, Signature|string, Extra|dict]
					callback = this.$wampClientCore.get_heartbeatCallback();
					if (!ss.staticEquals(callback, null)) {
						callback(data);
					}
					else {
					}
					break;
				}
				case 8: {
					// WAMP SPEC: [ERROR, REQUEST.Type|int, REQUEST.Request|id, Details|dict, Error|uri, (Arguments|list, ArgumentsKw|dict)]
					var switchDataSec = ss.unbox(ss.cast(list[1], ss.Int32));
					var request = ss.unbox(ss.cast(list[2], ss.Int32));
					switch (switchDataSec) {
						case 32: {
							// WAMP SPEC: [ERROR, SUBSCRIBE, SUBSCRIBE.Request|id, Details|dict, Error|uri]
							callback = this.$wampClientCore.get_subscribeCallback().get_item(request);
							if (!ss.staticEquals(callback, null)) {
								callback(data);
							}
							else {
							}
							break;
						}
						case 34: {
							// WAMP SPEC: [ERROR, UNSUBSCRIBE, UNSUBSCRIBE.Request|id, Details|dict, Error|uri]
							callback = this.$wampClientCore.get_unsubscribeCallback().get_item(request);
							if (!ss.staticEquals(callback, null)) {
								callback(data);
							}
							else {
							}
							break;
						}
						case 16: {
							// WAMP SPEC: [ERROR, PUBLISH, PUBLISH.Request|id, Details|dict, Error|uri]
							callback = this.$wampClientCore.get_publishCallback().get_item(request);
							if (!ss.staticEquals(callback, null)) {
								callback(data);
							}
							else {
							}
							break;
						}
						case 68: {
							// WAMP SPEC: [ERROR, INVOCATION, INVOCATION.Request|id, Details|dict, Error|uri]
							// WAMP SPEC: [ERROR, INVOCATION, INVOCATION.Request|id, Details|dict, Error|uri, Arguments|list]
							// WAMP SPEC: [ERROR, INVOCATION, INVOCATION.Request|id, Details|dict, Error|uri, Arguments|list, ArgumentsKw|dict]
							callback = this.$wampClientCore.get_invocationCallback().get_item(request);
							if (!ss.staticEquals(callback, null)) {
								callback(data);
							}
							else {
							}
							break;
						}
						case 48: {
							// WAMP SPEC: [ERROR, CALL, CALL.Request|id, Details|dict, Error|uri]
							// WAMP SPEC: [ERROR, CALL, CALL.Request|id, Details|dict, Error|uri, Arguments|list]
							// WAMP SPEC: [ERROR, CALL, CALL.Request|id, Details|dict, Error|uri, Arguments|list, ArgumentsKw|dict]
							callback = this.$wampClientCore.get_callCallback().get_item(request);
							if (!ss.staticEquals(callback, null)) {
								callback(data);
							}
							else {
							}
							break;
						}
						case 64: {
							// WAMP SPEC: [ERROR, REGISTER, REGISTER.Request|id, Details|dict, Error|uri]
							callback = this.$wampClientCore.get_registerCallback().get_item(request);
							if (!ss.staticEquals(callback, null)) {
								callback(data);
							}
							else {
							}
							break;
						}
						case 66: {
							// WAMP SPEC: [ERROR, UNREGISTER, UNREGISTER.Request|id, Details|dict, Error|uri]
							callback = this.$wampClientCore.get_unregisterCallback().get_item(request);
							if (!ss.staticEquals(callback, null)) {
								callback(data);
							}
							else {
							}
							break;
						}
						default: {
							return;
						}
					}
					break;
				}
				case 17: {
					// WAMP SPEC: [PUBLISHED, PUBLISH.Request|id, Publication|id]
					callback = this.$wampClientCore.get_publishCallback().get_item(ss.unbox(ss.cast(list[1], ss.Int32)));
					if (!ss.staticEquals(callback, null)) {
						callback(data);
					}
					else {
					}
					break;
				}
				case 33: {
					// WAMP SPEC: [SUBSCRIBED, SUBSCRIBE.Request|id, Subscription|id]
					var reqId = ss.unbox(ss.cast(list[1], ss.Int32));
					var subId = ss.unbox(ss.cast(list[2], ss.Int32));
					callback = this.$wampClientCore.get_subscribeCallback().get_item(reqId);
					if (!ss.staticEquals(callback, null)) {
						this.$wampClientCore.get_eventCallback().set_item(subId, this.$wampClientCore.get_eventCallback().get_item(reqId));
						this.$wampClientCore.deleteEventCallback(reqId);
						callback(data);
					}
					else {
					}
					break;
				}
				case 35: {
					// WAMP SPEC: [UNSUBSCRIBED, UNSUBSCRIBE.Request|id]
					callback = this.$wampClientCore.get_unsubscribeCallback().get_item(ss.unbox(ss.cast(list[1], ss.Int32)));
					if (!ss.staticEquals(callback, null)) {
						this.$wampClientCore.deleteEventCallback(this.$subscription_id_list.get_item(ss.unbox(ss.cast(list[1], ss.Int32))));
						callback(data);
					}
					else {
					}
					break;
				}
				case 36: {
					// WAMP SPEC: [EVENT, SUBSCRIBED.Subscription|id, PUBLISHED.Publication|id, Details|dict]
					// WAMP SPEC: [EVENT, SUBSCRIBED.Subscription|id, PUBLISHED.Publication|id, Details|dict, PUBLISH.Arguments|list]
					// WAMP SPEC: [EVENT, SUBSCRIBED.Subscription|id, PUBLISHED.Publication|id, Details|dict, PUBLISH.Arguments|list, PUBLISH.ArgumentKw|dict]
					callback = this.$wampClientCore.get_eventCallback().get_item(ss.unbox(ss.cast(list[1], ss.Int32)));
					if (!ss.staticEquals(callback, null)) {
						callback(data);
					}
					else {
					}
					break;
				}
				case 50: {
					// WAMP SPEC: [RESULT, CALL.Request|id, Details|dict]
					// WAMP SPEC: [RESULT, CALL.Request|id, Details|dict, YIELD.Arguments|list]
					// WAMP SPEC: [RESULT, CALL.Request|id, Details|dict, YIELD.Arguments|list, YIELD.ArgumentsKw|dict]
					callback = this.$wampClientCore.get_callCallback().get_item(ss.unbox(ss.cast(list[1], ss.Int32)));
					if (!ss.staticEquals(callback, null)) {
						callback(data);
					}
					else {
					}
					break;
				}
				case 65: {
					// WAMP SPEC: [REGISTERED, REGISTER.Request|id, Registration|id]
					callback = this.$wampClientCore.get_registerCallback().get_item(ss.unbox(ss.cast(list[1], ss.Int32)));
					if (!ss.staticEquals(callback, null)) {
						callback(data);
					}
					else {
					}
					break;
				}
				case 67: {
					// WAMP SPEC: [UNREGISTERED, UNREGISTER.Request|id]
					callback = this.$wampClientCore.get_unregisterCallback().get_item(ss.unbox(ss.cast(list[1], ss.Int32)));
					if (!ss.staticEquals(callback, null)) {
						callback(data);
					}
					else {
					}
					break;
				}
				case 68: {
					// WAMP SPEC: [INVOCATION, Request|id, REGISTERED.Registration|id, Details|dict]
					// WAMP SPEC: [INVOCATION, Request|id, REGISTERED.Registration|id, Details|dict, CALL.Arguments|list]
					// WAMP SPEC: [INVOCATION, Request|id, REGISTERED.Registration|id, Details|dict, CALL.Arguments|list, CALL.ArgumentsKw|dict]
					callback = this.$wampClientCore.get_invocationCallback().get_item(ss.unbox(ss.cast(list[1], ss.Int32)));
					if (!ss.staticEquals(callback, null)) {
						callback(data);
					}
					else {
					}
					break;
				}
				case 69: {
					// advanced WAMP SPEC: [INTERRUPT, INVOCATION.Request|id, Options|dict]
					callback = this.$wampClientCore.get_interruptCallback().get_item(ss.unbox(ss.cast(list[1], ss.Int32)));
					if (!ss.staticEquals(callback, null)) {
						callback(data);
					}
					else {
					}
					break;
				}
				default: {
					break;
				}
			}
		},
		isConnecting: function() {
			return this.$on_open_flag;
		}
	});
	ss.initClass($Kadecot_WampClientCore, $asm, {
		get_openCallback: function() {
			return this.$1$openCallbackField;
		},
		set_openCallback: function(value) {
			this.$1$openCallbackField = value;
		},
		get_closeCallback: function() {
			return this.$1$closeCallbackField;
		},
		set_closeCallback: function(value) {
			this.$1$closeCallbackField = value;
		},
		get_heartbeatCallback: function() {
			return this.$1$heartbeatCallbackField;
		},
		set_heartbeatCallback: function(value) {
			this.$1$heartbeatCallbackField = value;
		},
		get_helloCallback: function() {
			return this.$1$helloCallbackField;
		},
		set_helloCallback: function(value) {
			this.$1$helloCallbackField = value;
		},
		get_authenticateCallback: function() {
			return this.$1$authenticateCallbackField;
		},
		set_authenticateCallback: function(value) {
			this.$1$authenticateCallbackField = value;
		},
		get_goodbyeCallback: function() {
			return this.$1$goodbyeCallbackField;
		},
		set_goodbyeCallback: function(value) {
			this.$1$goodbyeCallbackField = value;
		},
		get_publishCallback: function() {
			return this.$1$publishCallbackField;
		},
		set_publishCallback: function(value) {
			this.$1$publishCallbackField = value;
		},
		get_subscribeCallback: function() {
			return this.$1$subscribeCallbackField;
		},
		set_subscribeCallback: function(value) {
			this.$1$subscribeCallbackField = value;
		},
		get_unsubscribeCallback: function() {
			return this.$1$unsubscribeCallbackField;
		},
		set_unsubscribeCallback: function(value) {
			this.$1$unsubscribeCallbackField = value;
		},
		get_callCallback: function() {
			return this.$1$callCallbackField;
		},
		set_callCallback: function(value) {
			this.$1$callCallbackField = value;
		},
		get_cancelCallback: function() {
			return this.$1$cancelCallbackField;
		},
		set_cancelCallback: function(value) {
			this.$1$cancelCallbackField = value;
		},
		get_registerCallback: function() {
			return this.$1$registerCallbackField;
		},
		set_registerCallback: function(value) {
			this.$1$registerCallbackField = value;
		},
		get_unregisterCallback: function() {
			return this.$1$unregisterCallbackField;
		},
		set_unregisterCallback: function(value) {
			this.$1$unregisterCallbackField = value;
		},
		get_eventCallback: function() {
			return this.$1$eventCallbackField;
		},
		set_eventCallback: function(value) {
			this.$1$eventCallbackField = value;
		},
		get_invocationCallback: function() {
			return this.$1$invocationCallbackField;
		},
		set_invocationCallback: function(value) {
			this.$1$invocationCallbackField = value;
		},
		get_interruptCallback: function() {
			return this.$1$interruptCallbackField;
		},
		set_interruptCallback: function(value) {
			this.$1$interruptCallbackField = value;
		},
		deleteOpenCallback: function() {
			this.set_openCallback(null);
		},
		deleteCloseCallback: function() {
			this.set_closeCallback(null);
		},
		deleteHeartbeatCallback: function() {
			this.set_heartbeatCallback(null);
		},
		deleteHelloCallback: function() {
			this.set_helloCallback(null);
		},
		deleteAuthenticateCallback: function() {
			this.set_authenticateCallback(null);
		},
		deleteGoodbyeCallback: function() {
			this.set_goodbyeCallback(null);
		},
		deletePublishCallback: function(request) {
			this.get_publishCallback().set_item(request, null);
		},
		deleteSubscribeCallback: function(request) {
			this.get_subscribeCallback().set_item(request, null);
		},
		deleteUnsubscribeCallback: function(request) {
			this.get_unsubscribeCallback().set_item(request, null);
		},
		deleteCallCallback: function(request) {
			this.get_callCallback().set_item(request, null);
		},
		deleteCancelCallback: function(request) {
			this.get_cancelCallback().set_item(request, null);
		},
		deleteRegisterCallback: function(request) {
			this.get_registerCallback().set_item(request, null);
		},
		deleteUnregisterCallback: function(request) {
			this.get_unregisterCallback().set_item(request, null);
		},
		deleteEventCallback: function(request) {
			this.get_eventCallback().set_item(request, null);
		},
		deleteInvocationCallback: function(request) {
			this.get_invocationCallback().set_item(request, null);
		},
		deleteInterruptCallback: function(request) {
			this.get_interruptCallback().set_item(request, null);
		}
	});
	ss.initClass($Kadecot_WampDeserializer, $asm, {});
	ss.initClass($Kadecot_WampSerializer, $asm, {});
	ss.setMetadata($ctrlui_EPC_FLAG, { enumFlags: true });
	(function() {
		$control_UIEventNames.currentNodeChange = 'currentnode.change';
		$control_UIEventNames.loadingShow = 'loading.show';
		$control_UIEventNames.loadingHide = 'loading.hide';
		$control_UIEventNames.searchStart = 'search.start';
		$control_UIEventNames.searchEnd = 'search.end';
		$control_UIEventNames.searchError = 'search.error';
		$control_UIEventNames.propertyWriteStart = 'propertywrite.start';
		$control_UIEventNames.propertyWriteEnd = 'propertywrite.end';
	})();
	(function() {
		$ctrlui_$TokenInfo.$separetor = '[=,:\\(\\)\\[\\]～]';
		$ctrlui_$TokenInfo.$hexValue = '^0x[0-9A-Fa-f]+';
		$ctrlui_$TokenInfo.$numeric = '^[\\-\\+]?[\\.]?[0-9]+[0-9,\\.]*';
	})();
	(function() {
		$ctrlui_ValueRange.$_ParsedText = new (ss.makeGenericType(ss.Dictionary$2, [String, $ctrlui_$ValueSet]))();
	})();
	(function() {
		$ctrlui_DeviceController.getClassInfo = null;
	})();
	(function() {
		$ctrlui_T_ECN_EOJ.x1_PROFILE = 14;
		$ctrlui_T_ECN_EOJ.x2_NODE_PROFILE = 240;
	})();
	(function() {
		$CtrlUI.page = null;
		$CtrlUI.info = null;
		$CtrlUI.$page = null;
		$CtrlUI.$m_ClassGroups = null;
		$CtrlUI.$m_ProfileClassInfo = null;
		$CtrlUI.$m_Classes = null;
	})();
})();
