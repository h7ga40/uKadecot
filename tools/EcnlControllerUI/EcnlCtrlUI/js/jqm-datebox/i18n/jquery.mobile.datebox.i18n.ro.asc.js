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
	'ro': {
		setDateButtonLabel: "Seta&#355;i data",
		setTimeButtonLabel: "Seta&#355;i timp",
		setDurationButtonLabel: "Seta&#355;i durata",
		calTodayButtonLabel: "Salt la azi",
		titleDateDialogLabel: "Alege&#355;i data",
		titleTimeDialogLabel: "Alege timp",
		daysOfWeek: ["Duminic&#259;", "Luni", "Mar&#355;i", "Miercuri", "Joi", "Vineri", "S&#226;mb&#259;t&#259;"],
		daysOfWeekShort: ["Dum", "Lun", "Mar", "Mie", "Joi", "Vin", "S&#226;m"],
		monthsOfYear: ["Ianuarie", "Februarie", "Martie", "Aprilie", "Mai", "Iunie", "Iulie", "August", "Septembrie", "Octombrie", "Noiembrie", "Decembrie"],
		monthsOfYearShort: ["Ian", "Feb", "Mar", "Apr", "Mai", "Iun", "Iul", "Aug", "Sep", "Oct", "Nov", "Dec"],
		durationLabel: ["Zi", "Ore", "Minute", "Secunde"],
		durationDays: ["Ziua", "Zi"],
		tooltip: "Deschide selectorul de dat&#259;",
		nextMonth: "Luna urm&#259;toare",
		prevMonth: "Luna precedent&#259;",
		timeFormat: 24,
		headerFormat: '%A, %B %-d, %Y',
		dateFieldOrder: ['d','m','y'],
		timeFieldOrder: ['h', 'i', 'a'],
		slideFieldOrder: ['y', 'm', 'd'],
		dateFormat: "%d.%m.%y",
		useArabicIndic: false,
		isRTL: false,
		calStartDay: 0,
		clearButton: "Clar",
		durationOrder: ['d', 'h', 'i', 's'],
		meridiem: ["AM", "PM"],
		timeOutput: "%k:%M",
		durationFormat: "%Dd %DA, %Dl:%DM:%DS",
		calDateListLabel: "Alte date",
		calHeaderFormat: "%B %Y"
	}
});
jQuery.extend(jQuery.mobile.datebox.prototype.options, {
	useLang: 'ro'
});

