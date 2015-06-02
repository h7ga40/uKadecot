/*
 * jQuery Mobile Framework : plugin to provide a date and time picker.
 * Copyright (c) JTSage
 * CC 3.0 Attribution.  May be relicensed without permission/notifcation.
 * https://github.com/jtsage/jquery-mobile-datebox
 *
 * Translation by: Unknown
 *
 */

jQuery.extend(jQuery.mobile.datebox.prototype.options.lang, {
	'vi': {
		setDateButtonLabel: "&#272;&#243;ng",
		setTimeButtonLabel: "&#272;&#243;ng",
		setDurationButtonLabel: "&#272;&#243;ng",
		calTodayButtonLabel: "H&#244;m nay",
		titleDateDialogLabel: "Ch&#7885;n ng&#224;y",
		titleTimeDialogLabel: "Ch&#7885;n th&#7901;i gian",
		daysOfWeek: ["Ch&#7911; Nh&#7853;t", "Th&#7913; hai", "Th&#7913; Ba", "Th&#7913; T&#432;", "Th&#7913; N&#259;m", "Th&#7913; s&#225;u", "Th&#7913; B&#7843;y"],
		daysOfWeekShort: ["CN", "T2", "T3", "T4", "T5", "T6", "T7"],
		monthsOfYear: ["Th&#225;ng m&#7897;t", "Th&#225;ng hai", "Th&#225;ng ba", "Th&#225;ng t&#432;", "Th&#225;ng n&#259;m", "Th&#225;ng S&#225;u", "Th&#225;ng B&#7843;y", "Th&#225;ng T&#225;m", "Th&#225;ng Ch&#237;n", "Th&#225;ng M&#432;&#7901;i", "Th&#225;ng m&#432;&#7901;i m&#7897;t", "Th&#225;ng m&#432;&#7901;i hai"],
		monthsOfYearShort: ["Th&#225;ng m&#7897;t", "Th&#225;ng hai", "Th&#225;ng ba", "Th&#225;ng t&#432;", "Th&#225;ng n&#259;m", "Th&#225;ng S&#225;u", "Th&#225;ng B&#7843;y", "Th&#225;ng T&#225;m", "Th&#225;ng Ch&#237;n", "Th&#225;ng M&#432;&#7901;i", "Th&#225;ng m&#432;&#7901;i m&#7897;t", "Th&#225;ng m&#432;&#7901;i hai"],
		durationLabel: ["Ng&#224;y", "Gi&#7901;", "Ph&#250;t", "Gi&#226;y"],
		durationDays: ["Ng&#224;y", "Ng&#224;y"],
		tooltip: "M&#7903; b&#7843;ng ch&#7885;n ng&#224;y",
		nextMonth: "Ti&#7871;p",
		prevMonth: "Tr&#432;&#7899;c",
		timeFormat: 24,
		headerFormat: '%A, %B %-d, %Y',
		dateFieldOrder: ['d','m','y'],
		timeFieldOrder: ['h', 'i', 'a'],
		slideFieldOrder: ['y', 'm', 'd'],
		dateFormat: "%d/%m/%Y",
		useArabicIndic: false,
		isRTL: false,
		calStartDay: 0,
		clearButton: "R&#245; r&#224;ng",
		durationOrder: ['d', 'h', 'i', 's'],
		meridiem: ["AM", "PM"],
		timeOutput: "%k:%M",
		durationFormat: "%Dd %DA, %Dl:%DM:%DS",
		calDateListLabel: "Ng&#224;y kh&#225;c",
		calHeaderFormat: "%B %Y"
	}
});
jQuery.extend(jQuery.mobile.datebox.prototype.options, {
	useLang: 'vi'
});

