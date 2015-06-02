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
		calTodayButtonLabel: "Bugün",
		titleDateDialogLabel: "Tarih seçin",
		titleTimeDialogLabel: "Seçin",
		daysOfWeek: ["Pazar", "Pazartesi", "Salı", "Çarşamba", "Perşembe", "Cuma", "Cumartesi"],
		daysOfWeekShort: ["Pz", "Pt", "Sa", "Ça", "Pe", "Cu", "Ct"],
		monthsOfYear: ["Ocak", "Şubat", "Mart", "Nisan", "Mayıs", "Haziran", "Temmuz", "Ağustos", "Eylül", "Ekim", "Kasım", "Aralık"],
		monthsOfYearShort: ["Oca", "Şub", "Mar", "Nis", "May", "Haz", "Tem", "Ağu", "Eyl", "Eki", "Kas", "Ara"],
		durationLabel: ["Gün", "Saat", "Dakika", "Saniye"],
		durationDays: ["Gün", "Gün"],
		tooltip: "Açık tarih seçici",
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
		clearButton: "Açık",
		durationOrder: ['d', 'h', 'i', 's'],
		meridiem: ["AM", "PM"],
		timeOutput: "%k:%M",
		durationFormat: "%Dd %DA, %Dl:%DM:%DS",
		calDateListLabel: "Diğer tarihler",
		calHeaderFormat: "%B %Y"
	}
});
jQuery.extend(jQuery.mobile.datebox.prototype.options, {
	useLang: 'tr'
});

