/*
 * jQuery Mobile Framework : plugin to provide a date and time picker.
 * Copyright (c) JTSage
 * CC 3.0 Attribution.  May be relicensed without permission/notifcation.
 * https://github.com/jtsage/jquery-mobile-datebox
 *
 * Translation by: Michael de Lara <mykel1829@yahoo.com>
 *
 */

jQuery.extend(jQuery.mobile.datebox.prototype.options.lang, {
	'ar': {
		setDateButtonLabel: "تعيين تاريخ",
		setTimeButtonLabel: "ضبط الوقت",
		setDurationButtonLabel: "تعيين المدة",
		calTodayButtonLabel: "القفز إلى اليوم",
		titleDateDialogLabel: "اختر التاريخ",
		titleTimeDialogLabel: "اختر الوقت",
		daysOfWeek: ["الأحد", "الاثنين", "الثلاثاء", "الاربعاء", "الخميس", "الجمعة", "السبت"],
		daysOfWeekShort: ["الأحد", "الاثنين", "الثلاثاء", "الاربعاء", "الخميس", "الجمعة", "السبت"],
		monthsOfYear: ["يناير", "فبراير", "مارس", "إبريل", "مايو", "يونية", "يولية", "أغسطس", "سبتمبر", "أكتوبر", "نوفمبر", "ديسمبر"],
		monthsOfYearShort: ["1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12"],
		durationLabel: ["أيام", "ساعة", "دقيقة", "ثانية"],
		durationDays: ["يوم", "أيام"],
		tooltip: "فتح منتقي التاريخ",
		nextMonth: "التالي",
		prevMonth: "السابق",
		timeFormat: 24,
		headerFormat: '%A, %B %-d, %Y',
		dateFieldOrder: ['d','m','y'],
		timeFieldOrder: ['h', 'i', 'a'],
		slideFieldOrder: ['y', 'm', 'd'],
		dateFormat: "%d/%m/%Y",
		useArabicIndic: true,
		isRTL: true,
		calStartDay: 0,
		clearButton: "واضحة",
		durationOrder: ['d', 'h', 'i', 's'],
		meridiem: ["AM", "PM"],
		timeOutput: "%k:%M",
		durationFormat: "%Dd %DA, %Dl:%DM:%DS",
		calDateListLabel: "تواريخ أخرى",
		calHeaderFormat: "%B %Y"
	}
});
jQuery.extend(jQuery.mobile.datebox.prototype.options, {
	useLang: 'ar'
});

