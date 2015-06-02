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
		setDateButtonLabel: "Setaţi data",
		setTimeButtonLabel: "Setaţi timp",
		setDurationButtonLabel: "Setaţi durata",
		calTodayButtonLabel: "Salt la azi",
		titleDateDialogLabel: "Alegeţi data",
		titleTimeDialogLabel: "Alege timp",
		daysOfWeek: ["Duminică", "Luni", "Marţi", "Miercuri", "Joi", "Vineri", "Sâmbătă"],
		daysOfWeekShort: ["Dum", "Lun", "Mar", "Mie", "Joi", "Vin", "Sâm"],
		monthsOfYear: ["Ianuarie", "Februarie", "Martie", "Aprilie", "Mai", "Iunie", "Iulie", "August", "Septembrie", "Octombrie", "Noiembrie", "Decembrie"],
		monthsOfYearShort: ["Ian", "Feb", "Mar", "Apr", "Mai", "Iun", "Iul", "Aug", "Sep", "Oct", "Nov", "Dec"],
		durationLabel: ["Zi", "Ore", "Minute", "Secunde"],
		durationDays: ["Ziua", "Zi"],
		tooltip: "Deschide selectorul de dată",
		nextMonth: "Luna următoare",
		prevMonth: "Luna precedentă",
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

