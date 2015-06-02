/*
 * jQuery Mobile Framework : plugin to provide a date and time picker.
 * Copyright (c) JTSage
 * CC 3.0 Attribution.  May be relicensed without permission/notifcation.
 * https://github.com/jtsage/jquery-mobile-datebox
 *
 * Translation by: Ethan Chen <ethan42411@gmail.com>
 *
 */

jQuery.extend(jQuery.mobile.datebox.prototype.options.lang, {
	'zh-TW': {
		setDateButtonLabel: "&#35373;&#23450;&#26085;&#26399;",
		setTimeButtonLabel: "&#35373;&#23450;&#26178;&#38291;",
		setDurationButtonLabel: "&#35373;&#23450;&#25345;&#32396;&#26085;&#26399;",
		calTodayButtonLabel: "&#36984;&#25799;&#20170;&#22825;&#26085;&#26399;",
		titleDateDialogLabel: "&#36984;&#25799;&#26085;&#26399;",
		titleTimeDialogLabel: "&#36984;&#25799;&#26178;&#38291;",
		daysOfWeek: ["&#26143;&#26399;&#26085;", "&#26143;&#26399;&#19968;", "&#26143;&#26399;&#20108;", "&#26143;&#26399;&#19977;", "&#26143;&#26399;&#22235;", "&#26143;&#26399;&#20116;", "&#26143;&#26399;&#20845;"],
		daysOfWeekShort: ["&#26085;", "&#19968;", "&#20108;", "&#19977;", "&#22235;", "&#20116;", "&#20845;"],
		monthsOfYear: ["&#19968;&#26376;", "&#20108;&#26376;", "&#19977;&#26376;", "&#22235;&#26376;", "&#20116;&#26376;", "&#20845;&#26376;", "&#19971;&#26376;", "&#20843;&#26376;", "&#20061;&#26376;", "&#21313;&#26376;", "&#21313;&#19968;&#26376;", "&#21313;&#20108;&#26376;"],
		monthsOfYearShort: ["&#19968;", "&#20108;", "&#19977;", "&#22235;", "&#20116;", "&#20845;", "&#19971;", "&#20843;", "&#20061;", "&#21313;", "&#21313;&#19968;", "&#21313;&#20108;"],
		durationLabel: ["&#22825;", "&#23567;&#26178;", "&#20998;&#37912;", "&#31186;"],
		durationDays: ["&#22825;", "&#22825;"],
		tooltip: "&#38283;&#21855;&#26085;&#26399;&#36984;&#25799;",
		nextMonth: "&#19979;&#20491;&#26376;",
		prevMonth: "&#19978;&#20491;&#26376;",
		timeFormat: 24,
		headerFormat: '%A, %B %-d, %Y',
		dateFieldOrder: ['m', 'd', 'y'],
		timeFieldOrder: ['h', 'i', 'a'],
		slideFieldOrder: ['y', 'm', 'd'],
		dateFormat: "%Y-%m-%d",
		useArabicIndic: false,
		isRTL: false,
		calStartDay: 0,
		clearButton: "&#28165;&#38500;",
		durationOrder: ['d', 'h', 'i', 's'],
		meridiem: ["&#19978;&#21320;", "&#19979;&#21320;"],
		timeOutput: "%k:%M",
		durationFormat: "%Dd %DA, %Dl:%DM:%DS",
		calDateListLabel: "&#20854;&#20182;&#26085;&#26399;",
		calHeaderFormat: "%B %Y"
	}
});
jQuery.extend(jQuery.mobile.datebox.prototype.options, {
	useLang: 'zh-TW'
});

