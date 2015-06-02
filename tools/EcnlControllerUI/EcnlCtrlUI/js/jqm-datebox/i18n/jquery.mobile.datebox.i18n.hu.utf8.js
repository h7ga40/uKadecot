/*
 * jQuery Mobile Framework : plugin to provide a date and time picker.
 * Copyright (c) JTSage
 * CC 3.0 Attribution.  May be relicensed without permission/notifcation.
 * https://github.com/jtsage/jquery-mobile-datebox
 *
 * Translation by: titititatatatititi <crowdin>
 *
 */

jQuery.extend(jQuery.mobile.datebox.prototype.options.lang, {
	'hu': {
		setDateButtonLabel: "Dátum választása",
		setTimeButtonLabel: "Idő választása",
		setDurationButtonLabel: "Időtartam beállítása",
		calTodayButtonLabel: "Ugrás a mai napra",
		titleDateDialogLabel: "Dátum kiválasztása",
		titleTimeDialogLabel: "Idő kiválasztása",
		daysOfWeek: ["Vasárnap", "Hétfő", "Kedd", "Szerda", "Csütörtök", "Péntek", "Szombat"],
		daysOfWeekShort: ["V", "H", "K", "Sze", "Cs", "P", "Szo"],
		monthsOfYear: ["Január", "Február", "Március", "Április", "Május", "Június", "Július", "Augusztus", "Szeptember", "Október", "November", "December"],
		monthsOfYearShort: ["Jan.", "Febr.", "Márc.", "Ápr.", "Máj.", "Jún.", "Júl.", "Aug.", "Szept.", "Okt.", "Nov.", "Dec."],
		durationLabel: ["Napok", "Óra", "Perc", "Másodperc"],
		durationDays: ["Nap", "Napok"],
		tooltip: "Dátumválasztó megnyitása",
		nextMonth: "Köv. hónap",
		prevMonth: "Előző hónap",
		timeFormat: 24,
		headerFormat: '%A, %B %-d, %Y',
		dateFieldOrder: ['y','m','d'],
		timeFieldOrder: ['h', 'i', 'a'],
		slideFieldOrder: ['y', 'm', 'd'],
		dateFormat: "%Y.%m.%d.",
		useArabicIndic: false,
		isRTL: false,
		calStartDay: 1,
		clearButton: "Törlés",
		durationOrder: ['d', 'h', 'i', 's'],
		meridiem: ["de.", "du."],
		timeOutput: "%k.%M",
		durationFormat: "%Dd %DA, %Dl:%DM:%DS",
		calDateListLabel: "Más időpontok",
		calHeaderFormat: "%B %Y"
	}
});
jQuery.extend(jQuery.mobile.datebox.prototype.options, {
	useLang: 'hu'
});

