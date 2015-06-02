/*
 * jQuery Mobile Framework : plugin to provide a date and time picker.
 * Copyright (c) JTSage
 * CC 3.0 Attribution.  May be relicensed without permission/notifcation.
 * https://github.com/jtsage/jquery-mobile-datebox
 *
 * Translation by: Jnyrup <crowdin>
 *
 */

jQuery.extend(jQuery.mobile.datebox.prototype.options.lang, {
	'da': {
		setDateButtonLabel: "Angiv dato",
		setTimeButtonLabel: "Angiv tid",
		setDurationButtonLabel: "Angiv varighed",
		calTodayButtonLabel: "G&#229; til i dag",
		titleDateDialogLabel: "V&#230;lg dato",
		titleTimeDialogLabel: "V&#230;lg tid",
		daysOfWeek: ["S&#248;ndag", "Mandag", "Tirsdag", "Onsdag", "Torsdag", "Fredag", "L&#248;rdag"],
		daysOfWeekShort: ["S&#248;n", "Man", "Tir", "Ons", "Tor", "Fre", "L&#248;r"],
		monthsOfYear: ["Januar", "Februar", "Marts", "April", "Maj", "Juni", "Juli", "August", "September", "Oktober", "November", "December"],
		monthsOfYearShort: ["Jan", "Feb", "Mar", "Apr", "Maj", "Jun", "Jul", "Aug", "Sep", "Okt", "Nov", "Dec"],
		durationLabel: ["Dage", "Timer", "Minutter", "Sekund"],
		durationDays: ["Dag", "Dage"],
		tooltip: "&#65279;&#197;bn datov&#230;lger",
		nextMonth: "N&#230;ste m&#229;ned",
		prevMonth: "Forrige m&#229;ned",
		timeFormat: 24,
		headerFormat: '%A, %B %-d, %Y',
		dateFieldOrder: ['d','m','y'],
		timeFieldOrder: ['h', 'i', 'a'],
		slideFieldOrder: ['y', 'm', 'd'],
		dateFormat: "%d-%m-%Y",
		useArabicIndic: false,
		isRTL: false,
		calStartDay: 0,
		clearButton: "Nulstil",
		durationOrder: ['d', 'h', 'i', 's'],
		meridiem: ["AM", "PM"],
		timeOutput: "%k:%M",
		durationFormat: "%Dd %DA, %Dl:%DM:%DS",
		calDateListLabel: "Andre datoer",
		calHeaderFormat: "%B %Y"
	}
});
jQuery.extend(jQuery.mobile.datebox.prototype.options, {
	useLang: 'da'
});

