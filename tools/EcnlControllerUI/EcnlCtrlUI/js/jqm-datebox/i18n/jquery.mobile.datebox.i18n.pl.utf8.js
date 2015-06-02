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
		setDateButtonLabel: "Ustaw datę",
		setTimeButtonLabel: "Ustaw godzinę",
		setDurationButtonLabel: "Ustaw okres",
		calTodayButtonLabel: "Dzisiaj",
		titleDateDialogLabel: "Wybierz datę",
		titleTimeDialogLabel: "Wybierz czas",
		daysOfWeek: ["Niedziela", "Poniedziałek", "Wtorek", "Środa", "Czwartek", "Piątek", "Sobota"],
		daysOfWeekShort: ["Nd", "Pn", "Wt", "Śr", "Cz", "Pt", "Sb"],
		monthsOfYear: ["Styczeń", "Luty", "Marzec", "Kwiecień", "Maj", "Czerwiec", "Lipiec", "Sierpień", "Wrzesień", "Październik", "Listopad", "Grudzień"],
		monthsOfYearShort: ["Sty", "Lut", "Mar", "Kwi", "Maj", "Cze", "Lip", "Sie", "Wrz", "Paź", "Lis", "Gru"],
		durationLabel: ["Dni", "Godziny", "Minuty", "Sekundy"],
		durationDays: ["Dzień", "Dni"],
		tooltip: "Otwórz wybór daty",
		nextMonth: "Następny miesiąc",
		prevMonth: "Poprzedni miesiąc",
		timeFormat: 24,
		headerFormat: '%A, %B %-d, %Y',
		dateFieldOrder: ['d','m','y'],
		timeFieldOrder: ['h', 'i', 'a'],
		slideFieldOrder: ['y', 'm', 'd'],
		dateFormat: "%Y-%m-%d",
		useArabicIndic: false,
		isRTL: false,
		calStartDay: 0,
		clearButton: "Wyczyść",
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

