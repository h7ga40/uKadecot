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
	'af': {
		setDateButtonLabel: "Selekteer",
		setTimeButtonLabel: "Selekteer",
		setDurationButtonLabel: "Selekteer",
		calTodayButtonLabel: "Vandag",
		titleDateDialogLabel: "Kies Datum",
		titleTimeDialogLabel: "Kies Tyd",
		daysOfWeek: ["Sondag", "Maandag", "Dinsdag", "Woensdag", "Donderdag", "Vrydag", "Saterdag"],
		daysOfWeekShort: ["Son", "Maa", "Din", "Woe", "Don", "Vry", "Sat"],
		monthsOfYear: ["Januarie", "Februarie", "Maart", "April", "Mei", "Junie", "Julie", "Augustus", "September", "Oktober", "November", "Desember"],
		monthsOfYearShort: ["Jan", "Feb", "Mrt", "Apr", "Mei", "Jun", "Jul", "Aug", "Sep", "Okt", "Nov", "Des"],
		durationLabel: ["Dae", "Ure", "Minute", "Sekondes"],
		durationDays: ["Dag", "Dae"],
		tooltip: "Maak Datumselekteerder",
		nextMonth: "Volgende",
		prevMonth: "Vorige",
		timeFormat: 24,
		headerFormat: '%A, %B %-d, %Y',
		dateFieldOrder: ['d','m','y'],
		timeFieldOrder: ['h', 'i', 'a'],
		slideFieldOrder: ['y', 'm', 'd'],
		dateFormat: "%d/%m/%Y",
		useArabicIndic: false,
		isRTL: false,
		calStartDay: 0,
		clearButton: "Duidelik",
		durationOrder: ['d', 'h', 'i', 's'],
		meridiem: ["AM", "PM"],
		timeOutput: "%k:%M",
		durationFormat: "%Dd %DA, %Dl:%DM:%DS",
		calDateListLabel: "Ander Datums",
		calHeaderFormat: "%B %Y"
	}
});
jQuery.extend(jQuery.mobile.datebox.prototype.options, {
	useLang: 'af'
});

