/*
 * jQuery Mobile Framework : plugin to provide a date and time picker.
 * Copyright (c) JTSage
 * CC 3.0 Attribution.  May be relicensed without permission/notifcation.
 * https://github.com/jtsage/jquery-mobile-datebox
 *
 * Translation by: Zbigniew Motyka <zbigniew@motyka.net.pl>
 *
 */

jQuery.extend(jQuery.mobile.datebox.prototype.options.lang, {
	'pl': {
		setDateButtonLabel: "Ustaw dat&#281;",
		setTimeButtonLabel: "Ustaw godzin&#281;",
		setDurationButtonLabel: "Ustaw okres",
		calTodayButtonLabel: "Dzisiaj",
		titleDateDialogLabel: "Wybierz dat&#281;",
		titleTimeDialogLabel: "Wybierz czas",
		daysOfWeek: ["Niedziela", "Poniedzia&#322;ek", "Wtorek", "&#346;roda", "Czwartek", "Pi&#261;tek", "Sobota"],
		daysOfWeekShort: ["Nd", "Pn", "Wt", "&#346;r", "Cz", "Pt", "Sb"],
		monthsOfYear: ["Stycze&#324;", "Luty", "Marzec", "Kwiecie&#324;", "Maj", "Czerwiec", "Lipiec", "Sierpie&#324;", "Wrzesie&#324;", "Pa&#378;dziernik", "Listopad", "Grudzie&#324;"],
		monthsOfYearShort: ["Sty", "Lut", "Mar", "Kwi", "Maj", "Cze", "Lip", "Sie", "Wrz", "Pa&#378;", "Lis", "Gru"],
		durationLabel: ["Dni", "Godziny", "Minuty", "Sekundy"],
		durationDays: ["Dzie&#324;", "Dni"],
		tooltip: "Otw&#243;rz wyb&#243;r daty",
		nextMonth: "Nast&#281;pny miesi&#261;c",
		prevMonth: "Poprzedni miesi&#261;c",
		timeFormat: 24,
		headerFormat: '%A, %B %-d, %Y',
		dateFieldOrder: ['d','m','y'],
		timeFieldOrder: ['h', 'i', 'a'],
		slideFieldOrder: ['y', 'm', 'd'],
		dateFormat: "%Y-%m-%d",
		useArabicIndic: false,
		isRTL: false,
		calStartDay: 0,
		clearButton: "Wyczy&#347;&#263;",
		durationOrder: ['d', 'h', 'i', 's'],
		meridiem: ["AM", "PM"],
		timeOutput: "%k:%M",
		durationFormat: "%Dd %DA, %Dl:%DM:%DS",
		calDateListLabel: "Inne daty",
		calHeaderFormat: "%B %Y"
	}
});
jQuery.extend(jQuery.mobile.datebox.prototype.options, {
	useLang: 'pl'
});

