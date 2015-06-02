/*
 * jQuery Mobile Framework : plugin to provide a date and time picker.
 * Copyright (c) JTSage
 * CC 3.0 Attribution.  May be relicensed without permission/notifcation.
 * https://github.com/jtsage/jquery-mobile-datebox
 *
 * Translation by: Kobi Hikri <kobi.hikri@gmail.com>
 *
 */

jQuery.extend(jQuery.mobile.datebox.prototype.options.lang, {
	'he': {
		setDateButtonLabel: "קבע תאריך",
		setTimeButtonLabel: "קבע זמן",
		setDurationButtonLabel: "קבע משך זמן",
		calTodayButtonLabel: "﻿קפיצה אל היום",
		titleDateDialogLabel: "בחר תאריך",
		titleTimeDialogLabel: "בחר זמן",
		daysOfWeek: ["ראשון", "שני", "שלישי", "רביעי", "חמישי", "שישי", "שבת"],
		daysOfWeekShort: ["ראש", "שני", "שלי", "רבי", "חמי", "ש", "שב"],
		monthsOfYear: ["ינואר", "פברואר", "מרץ", "אפריל", "מאי", "יוני", "יולי", "אוגוסט", "ספטמבר", "אוקטובר", "נובמבר", "דצמבר"],
		monthsOfYearShort: ["ינו", "פבר", "מרץ", "אפר", "מאי", "יונ", "יול", "אוג", "ספט", "אוק", "נוב", "דצמ"],
		durationLabel: ["ימים", "שעות", "דקות", "שניות"],
		durationDays: ["יום", "ימים"],
		tooltip: "פתח תאריכון",
		nextMonth: "החודש הבא",
		prevMonth: "החודש הקודם",
		timeFormat: 24,
		headerFormat: '%A, %B %-d, %Y',
		dateFieldOrder: ['m', 'd', 'y'],
		timeFieldOrder: ['h', 'i', 'a'],
		slideFieldOrder: ['y', 'm', 'd'],
		dateFormat: "%d-%m-%Y",
		useArabicIndic: false,
		isRTL: true,
		calStartDay: 0,
		clearButton: "נקה",
		durationOrder: ['d', 'h', 'i', 's'],
		meridiem: ["AM", "PM"],
		timeOutput: "%k:%M",
		durationFormat: "%Dd %DA, %Dl:%DM:%DS",
		calDateListLabel: "תאריכים אחרים",
		calHeaderFormat: "%B %Y"
	}
});
jQuery.extend(jQuery.mobile.datebox.prototype.options, {
	useLang: 'he'
});

