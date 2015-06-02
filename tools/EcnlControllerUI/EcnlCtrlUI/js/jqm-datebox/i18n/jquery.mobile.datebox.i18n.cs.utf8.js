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
	'cs': {
		setDateButtonLabel: "Nastavit datum",
		setTimeButtonLabel: "Nastavit čas",
		setDurationButtonLabel: "Nastavit dobu trvání",
		calTodayButtonLabel: "Nyní",
		titleDateDialogLabel: "Zvolte datum",
		titleTimeDialogLabel: "Zvolit čas",
		daysOfWeek: ["Neděle", "Pondělí", "Úterý", "Středa", "Čtvrtek", "Pátek", "Sobota"],
		daysOfWeekShort: ["Ne", "Po", "Út", "St", "Čt", "Pá", "So"],
		monthsOfYear: ["Leden", "Únor", "Březen", "Duben", "Květen", "Červen", "Červenec", "Srpen", "Září", "Říjen", "Listopad", "Prosinec"],
		monthsOfYearShort: ["Led", "Úno", "Bře", "Dub", "Kvě", "Čer", "Čvc", "Srp", "Zář", "Říj", "Lis", "Pro"],
		durationLabel: ["Dny", "Hodin", "Minut", "Sekundy"],
		durationDays: ["Den", "Dny"],
		tooltip: "Otevřít výběr data",
		nextMonth: "Později",
		prevMonth: "Dříve",
		timeFormat: 24,
		headerFormat: '%A, %B %-d, %Y',
		dateFieldOrder: ['d','m','y'],
		timeFieldOrder: ['h', 'i', 'a'],
		slideFieldOrder: ['y', 'm', 'd'],
		dateFormat: "%d.%m.%Y",
		useArabicIndic: false,
		isRTL: false,
		calStartDay: 0,
		clearButton: "Vymazat",
		durationOrder: ['d', 'h', 'i', 's'],
		meridiem: ["AM", "PM"],
		timeOutput: "%k:%M",
		durationFormat: "%Dd %DA, %Dl:%DM:%DS",
		calDateListLabel: "Další termíny",
		calHeaderFormat: "%B %Y"
	}
});
jQuery.extend(jQuery.mobile.datebox.prototype.options, {
	useLang: 'cs'
});

