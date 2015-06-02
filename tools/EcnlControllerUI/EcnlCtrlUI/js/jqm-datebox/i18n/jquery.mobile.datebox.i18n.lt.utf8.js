/*
 * jQuery Mobile Framework : plugin to provide a date and time picker.
 * Copyright (c) JTSage
 * CC 3.0 Attribution.  May be relicensed without permission/notifcation.
 * https://github.com/jtsage/jquery-mobile-datebox
 *
 * Translation by: Tadas Subonis <tadas.subonis@affecto.com>
 *
 */

jQuery.extend(jQuery.mobile.datebox.prototype.options.lang, {
	'lt': {
		setDateButtonLabel: "Data",
		setTimeButtonLabel: "Laikas",
		setDurationButtonLabel: "Trukmė",
		calTodayButtonLabel: "Šiandiena",
		titleDateDialogLabel: "Pasirinkti datą",
		titleTimeDialogLabel: "Pasirinkite laiko",
		daysOfWeek: ["Sekmadienis", "Pirmadienis", "Antradienis", "Trečiadienis", "Ketvirtadienis", "Penktadienis", "Šeštadienis"],
		daysOfWeekShort: ["Sk", "Pr", "An", "Tr", "Kt", "Pn", "Ss"],
		monthsOfYear: ["Sausis", "Vasaris", "Kovas", "Balandis", "Gegužė", "Birželis", "Liepa", "Rugpjūtis", "Rugsėjis", "Spalis", "Lapkritis", "Gruodis"],
		monthsOfYearShort: ["Sau", "Vas", "Kov", "Bal", "Geg", "Bir", "Lie", "Rug", "Rgs", "Spa", "Lap", "Gru"],
		durationLabel: ["Dienos", "Valandos", "Minutės", "Sekundeės"],
		durationDays: ["Diena", "Dienos"],
		tooltip: "Atidaryti datos parinkiklis",
		nextMonth: "Atgal",
		prevMonth: "Pirmyn",
		timeFormat: 24,
		headerFormat: '%A, %B %-d, %Y',
		dateFieldOrder: ['y', 'm', 'd'],
		timeFieldOrder: ['h', 'i', 'a'],
		slideFieldOrder: ['y', 'm', 'd'],
		dateFormat: "%Y-%m-%d",
		useArabicIndic: false,
		isRTL: false,
		calStartDay: 0,
		clearButton: "Aiškus",
		durationOrder: ['d', 'h', 'i', 's'],
		meridiem: ["AM", "PM"],
		timeOutput: "%k:%M",
		durationFormat: "%Dd %DA, %Dl:%DM:%DS",
		calDateListLabel: "Kitas datas",
		calHeaderFormat: "%B %Y"
	}
});
jQuery.extend(jQuery.mobile.datebox.prototype.options, {
	useLang: 'lt'
});

