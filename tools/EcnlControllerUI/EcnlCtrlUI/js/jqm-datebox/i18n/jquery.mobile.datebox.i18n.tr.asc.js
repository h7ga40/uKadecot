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
	'tr': {
		setDateButtonLabel: "Kapat",
		setTimeButtonLabel: "Kapat",
		setDurationButtonLabel: "Kapat",
		calTodayButtonLabel: "Bug&#252;n",
		titleDateDialogLabel: "Tarih se&#231;in",
		titleTimeDialogLabel: "Se&#231;in",
		daysOfWeek: ["Pazar", "Pazartesi", "Sal&#305;", "&#199;ar&#351;amba", "Per&#351;embe", "Cuma", "Cumartesi"],
		daysOfWeekShort: ["Pz", "Pt", "Sa", "&#199;a", "Pe", "Cu", "Ct"],
		monthsOfYear: ["Ocak", "&#350;ubat", "Mart", "Nisan", "May&#305;s", "Haziran", "Temmuz", "A&#287;ustos", "Eyl&#252;l", "Ekim", "Kas&#305;m", "Aral&#305;k"],
		monthsOfYearShort: ["Oca", "&#350;ub", "Mar", "Nis", "May", "Haz", "Tem", "A&#287;u", "Eyl", "Eki", "Kas", "Ara"],
		durationLabel: ["G&#252;n", "Saat", "Dakika", "Saniye"],
		durationDays: ["G&#252;n", "G&#252;n"],
		tooltip: "A&#231;&#305;k tarih se&#231;ici",
		nextMonth: "Ileri",
		prevMonth: "Geri",
		timeFormat: 24,
		headerFormat: '%A, %B %-d, %Y',
		dateFieldOrder: ['d','m','y'],
		timeFieldOrder: ['h', 'i', 'a'],
		slideFieldOrder: ['y', 'm', 'd'],
		dateFormat: "%d.%m.%Y",
		useArabicIndic: false,
		isRTL: false,
		calStartDay: 0,
		clearButton: "A&#231;&#305;k",
		durationOrder: ['d', 'h', 'i', 's'],
		meridiem: ["AM", "PM"],
		timeOutput: "%k:%M",
		durationFormat: "%Dd %DA, %Dl:%DM:%DS",
		calDateListLabel: "Di&#287;er tarihler",
		calHeaderFormat: "%B %Y"
	}
});
jQuery.extend(jQuery.mobile.datebox.prototype.options, {
	useLang: 'tr'
});

