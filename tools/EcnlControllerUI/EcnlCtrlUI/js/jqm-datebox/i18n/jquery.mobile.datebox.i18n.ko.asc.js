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
	'ko': {
		setDateButtonLabel: "&#45803;&#44592;",
		setTimeButtonLabel: "&#45803;&#44592;",
		setDurationButtonLabel: "&#45803;&#44592;",
		calTodayButtonLabel: "&#65279;&#50724;&#45720; &#51060;&#46041;",
		titleDateDialogLabel: "&#45216;&#51676; &#49440;&#53469;",
		titleTimeDialogLabel: "&#49884;&#44036; &#49440;&#53469;",
		daysOfWeek: ["&#51068;&#50836;&#51068;", "&#50900;&#50836;&#51068;", "&#54868;&#50836;&#51068;", "&#49688;&#50836;&#51068;", "&#47785;&#50836;&#51068;", "&#44552;&#50836;&#51068;", "&#53664;&#50836;&#51068;"],
		daysOfWeekShort: ["&#51068;", "&#50900;", "&#54868;", "&#49688;", "&#47785;", "&#44552;", "&#53664;"],
		monthsOfYear: ["1&#50900;", "2&#50900;", "3&#50900;", "4&#50900;", "5&#50900;", "6&#50900;", "7&#50900;", "8&#50900;", "9&#50900;", "10&#50900;", "11&#50900;", "12&#50900;"],
		monthsOfYearShort: ["1&#50900;", "2&#50900;", "3&#50900;", "4&#50900;", "5&#50900;", "6&#50900;", "7&#50900;", "8&#50900;", "9&#50900;", "10&#50900;", "11&#50900;", "12&#50900;"],
		durationLabel: ["&#45216;&#51676;", "&#49884;&#44036;", "&#48516;", "&#52488;"],
		durationDays: ["&#54616;&#47336;", "&#45216;&#51676;"],
		tooltip: "&#65279;&#50724;&#54536; &#45216;&#51676; &#49440;&#53469;",
		nextMonth: "&#65279;&#45796;&#51020; &#45804;",
		prevMonth: "&#65279;&#51060;&#51204; &#45804;",
		timeFormat: 24,
		headerFormat: '%A, %B %-d, %Y',
		dateFieldOrder: ['y','m','d'],
		timeFieldOrder: ['h', 'i', 'a'],
		slideFieldOrder: ['y', 'm', 'd'],
		dateFormat: "%Y-%m-%d",
		useArabicIndic: false,
		isRTL: false,
		calStartDay: 0,
		clearButton: "&#51648;&#50864;&#44592;",
		durationOrder: ['d', 'h', 'i', 's'],
		meridiem: ["AM", "PM"],
		timeOutput: "%k:%M",
		durationFormat: "%Dd %DA, %Dl:%DM:%DS",
		calDateListLabel: "&#45796;&#47480; &#45216;&#51676;",
		calHeaderFormat: "%B %Y"
	}
});
jQuery.extend(jQuery.mobile.datebox.prototype.options, {
	useLang: 'ko'
});

