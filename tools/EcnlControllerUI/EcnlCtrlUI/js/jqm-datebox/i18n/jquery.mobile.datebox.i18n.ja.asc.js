/*
 * jQuery Mobile Framework : plugin to provide a date and time picker.
 * Copyright (c) JTSage
 * CC 3.0 Attribution.  May be relicensed without permission/notifcation.
 * https://github.com/jtsage/jquery-mobile-datebox
 *
 * Translation by: Moon Dial
 *
 */

jQuery.extend(jQuery.mobile.datebox.prototype.options.lang, {
	'ja': {
		setDateButtonLabel: "&#26085;&#20184;&#12434;&#35373;&#23450;",
		setTimeButtonLabel: "&#26178;&#21051;&#12434;&#35373;&#23450;",
		setDurationButtonLabel: "&#26399;&#38291;&#12434;&#35373;&#23450;",
		calTodayButtonLabel: "&#20170;&#26085;&#12408;&#12472;&#12515;&#12531;&#12503;",
		titleDateDialogLabel: "&#26085;&#20184;&#12434;&#36984;&#25246;&#12375;&#12414;&#12377;&#12290;",
		titleTimeDialogLabel: "&#26178;&#38291;&#12434;&#36984;&#25246;&#12375;&#12414;&#12377;&#12290;",
		daysOfWeek: ["&#26085;&#26332;", "&#26376;&#26332;", "&#28779;&#26332;", "&#27700;&#26332;", "&#26408;&#26332;", "&#37329;&#26332;", "&#22303;&#26332;"],
		daysOfWeekShort: ["&#26085;", "&#26376;", "&#28779;", "&#27700;", "&#26408;", "&#37329;", "&#22303;"],
		monthsOfYear: ["1&#26376;", "2&#26376;", "3&#26376;", "4&#26376;", "5&#26376;", "6&#26376;", "7&#26376;", "8&#26376;", "9&#26376;", "10&#26376;", "11&#26376;", "12&#26376;"],
		monthsOfYearShort: ["1&#26376;", "2&#26376;", "3&#26376;", "4&#26376;", "5&#26376;", "6&#26376;", "7&#26376;", "8&#26376;", "9&#26376;", "10&#26376;", "11&#26376;", "12&#26376;"],
		durationLabel: ["&#26085;", "&#26178;&#38291;", "&#20998;", "&#31186;"],
		durationDays: ["&#26085;", "&#26085;"],
		tooltip: "&#26085;&#20184;&#12434;&#36984;&#25246;&#12377;&#12427;",
		nextMonth: "&#65279;&#27425;&#12398;&#26376;",
		prevMonth: "&#65279;&#21069;&#26376;",
		timeFormat: 24,
		headerFormat: '%A, %B %-d, %Y',
		dateFieldOrder: ['m', 'd', 'y'],
		timeFieldOrder: ['h', 'i', 'a'],
		slideFieldOrder: ['y', 'm', 'd'],
		dateFormat: "%Y/%m/%d",
		useArabicIndic: false,
		isRTL: false,
		calStartDay: 0,
		clearButton: "&#12463;&#12522;&#12450;",
		durationOrder: ['d', 'h', 'i', 's'],
		meridiem: ["AM", "PM"],
		timeOutput: "%k:%M",
		durationFormat: "%Dd %DA, %Dl:%DM:%DS",
		calDateListLabel: "&#12381;&#12398;&#20182;&#12398;&#26085;&#20184;",
		calHeaderFormat: "%B %Y"
	}
});
jQuery.extend(jQuery.mobile.datebox.prototype.options, {
	useLang: 'ja'
});

