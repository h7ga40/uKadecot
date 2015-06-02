/*
 * jQuery Mobile Framework : plugin to provide a date and time picker.
 * Copyright (c) JTSage
 * CC 3.0 Attribution.  May be relicensed without permission/notifcation.
 * https://github.com/jtsage/jquery-mobile-datebox
 *
 * Translation by: Marko <ma3ko0@gmail.com>
 *
 */

jQuery.extend(jQuery.mobile.datebox.prototype.options.lang, {
	'hr': {
		setDateButtonLabel: "Postavi Datum",
		setTimeButtonLabel: "Postavi Vrijeme",
		setDurationButtonLabel: "Postavi Trajanje",
		calTodayButtonLabel: "Dana&#353;nji Datum",
		titleDateDialogLabel: "Odaberite datum",
		titleTimeDialogLabel: "Odaberite vrijeme",
		daysOfWeek: ["Nedjelja", "Ponedjeljak", "Utorak", "Srijeda", "&#268;etvrtak", "Petak", "Subota"],
		daysOfWeekShort: ["Ne", "Po", "Ut", "Sr", "&#268;e", "Pe", "Su"],
		monthsOfYear: ["Sije&#269;anj", "Velja&#269;a", "O&#382;ujak", "Travanj", "Svibanj", "Lipanj", "Srpanj", "Kolovoz", "Rujan", "Listopad", "Studeni", "Prosinac"],
		monthsOfYearShort: ["Sij", "Vel", "O&#382;u", "Tra", "Svi", "Lip", "Srp", "Kol", "Ruj", "Lis", "Stu", "Pro"],
		durationLabel: ["Dani", "Sati", "Minute", "Sekunde"],
		durationDays: ["Dan", "Dani"],
		tooltip: "Otvorite kontrolu",
		nextMonth: "Sljede&#263;i",
		prevMonth: "Prethodna",
		timeFormat: 12,
		headerFormat: '%A, %B %-d, %Y',
		dateFieldOrder: ['d','m','y'],
		timeFieldOrder: ['h', 'i', 'a'],
		slideFieldOrder: ['y', 'm', 'd'],
		dateFormat: "%d.%m.%Y",
		useArabicIndic: false,
		isRTL: false,
		calStartDay: 0,
		clearButton: "Izbri&#353;i",
		durationOrder: ['d', 'h', 'i', 's'],
		meridiem: ["AM", "PM"],
		timeOutput: "%l:%M %p",
		durationFormat: "%Dd %DA, %Dl:%DM:%DS",
		calDateListLabel: "Ostali datumi",
		calHeaderFormat: "%B %Y"
	}
});
jQuery.extend(jQuery.mobile.datebox.prototype.options, {
	useLang: 'hr'
});

