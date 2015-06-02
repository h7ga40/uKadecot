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
		setTimeButtonLabel: "Nastavit &#269;as",
		setDurationButtonLabel: "Nastavit dobu trv&#225;n&#237;",
		calTodayButtonLabel: "Nyn&#237;",
		titleDateDialogLabel: "Zvolte datum",
		titleTimeDialogLabel: "Zvolit &#269;as",
		daysOfWeek: ["Ned&#283;le", "Pond&#283;l&#237;", "&#218;ter&#253;", "St&#345;eda", "&#268;tvrtek", "P&#225;tek", "Sobota"],
		daysOfWeekShort: ["Ne", "Po", "&#218;t", "St", "&#268;t", "P&#225;", "So"],
		monthsOfYear: ["Leden", "&#218;nor", "B&#345;ezen", "Duben", "Kv&#283;ten", "&#268;erven", "&#268;ervenec", "Srpen", "Z&#225;&#345;&#237;", "&#344;&#237;jen", "Listopad", "Prosinec"],
		monthsOfYearShort: ["Led", "&#218;no", "B&#345;e", "Dub", "Kv&#283;", "&#268;er", "&#268;vc", "Srp", "Z&#225;&#345;", "&#344;&#237;j", "Lis", "Pro"],
		durationLabel: ["Dny", "Hodin", "Minut", "Sekundy"],
		durationDays: ["Den", "Dny"],
		tooltip: "Otev&#345;&#237;t v&#253;b&#283;r data",
		nextMonth: "Pozd&#283;ji",
		prevMonth: "D&#345;&#237;ve",
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
		calDateListLabel: "Dal&#353;&#237; term&#237;ny",
		calHeaderFormat: "%B %Y"
	}
});
jQuery.extend(jQuery.mobile.datebox.prototype.options, {
	useLang: 'cs'
});

