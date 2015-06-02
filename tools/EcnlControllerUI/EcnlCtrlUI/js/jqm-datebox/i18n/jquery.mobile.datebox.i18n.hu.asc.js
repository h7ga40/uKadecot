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
		setDateButtonLabel: "D&#225;tum v&#225;laszt&#225;sa",
		setTimeButtonLabel: "Id&#337; v&#225;laszt&#225;sa",
		setDurationButtonLabel: "Id&#337;tartam be&#225;ll&#237;t&#225;sa",
		calTodayButtonLabel: "Ugr&#225;s a mai napra",
		titleDateDialogLabel: "D&#225;tum kiv&#225;laszt&#225;sa",
		titleTimeDialogLabel: "Id&#337; kiv&#225;laszt&#225;sa",
		daysOfWeek: ["Vas&#225;rnap", "H&#233;tf&#337;", "Kedd", "Szerda", "Cs&#252;t&#246;rt&#246;k", "P&#233;ntek", "Szombat"],
		daysOfWeekShort: ["V", "H", "K", "Sze", "Cs", "P", "Szo"],
		monthsOfYear: ["Janu&#225;r", "Febru&#225;r", "M&#225;rcius", "&#193;prilis", "M&#225;jus", "J&#250;nius", "J&#250;lius", "Augusztus", "Szeptember", "Okt&#243;ber", "November", "December"],
		monthsOfYearShort: ["Jan.", "Febr.", "M&#225;rc.", "&#193;pr.", "M&#225;j.", "J&#250;n.", "J&#250;l.", "Aug.", "Szept.", "Okt.", "Nov.", "Dec."],
		durationLabel: ["Napok", "&#211;ra", "Perc", "M&#225;sodperc"],
		durationDays: ["Nap", "Napok"],
		tooltip: "D&#225;tumv&#225;laszt&#243; megnyit&#225;sa",
		nextMonth: "K&#246;v. h&#243;nap",
		prevMonth: "El&#337;z&#337; h&#243;nap",
		timeFormat: 24,
		headerFormat: '%A, %B %-d, %Y',
		dateFieldOrder: ['y','m','d'],
		timeFieldOrder: ['h', 'i', 'a'],
		slideFieldOrder: ['y', 'm', 'd'],
		dateFormat: "%Y.%m.%d.",
		useArabicIndic: false,
		isRTL: false,
		calStartDay: 1,
		clearButton: "T&#246;rl&#233;s",
		durationOrder: ['d', 'h', 'i', 's'],
		meridiem: ["de.", "du."],
		timeOutput: "%k.%M",
		durationFormat: "%Dd %DA, %Dl:%DM:%DS",
		calDateListLabel: "M&#225;s id&#337;pontok",
		calHeaderFormat: "%B %Y"
	}
});
jQuery.extend(jQuery.mobile.datebox.prototype.options, {
	useLang: 'hu'
});

