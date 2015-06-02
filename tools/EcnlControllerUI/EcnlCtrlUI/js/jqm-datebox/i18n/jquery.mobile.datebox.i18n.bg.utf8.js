/*
 * jQuery Mobile Framework : plugin to provide a date and time picker.
 * Copyright (c) JTSage
 * CC 3.0 Attribution.  May be relicensed without permission/notifcation.
 * https://github.com/jtsage/jquery-mobile-datebox
 *
 * Translation by: How Bizarre <how@errazib.com>
 *
 */

jQuery.extend(jQuery.mobile.datebox.prototype.options.lang, {
	'bg': {
		setDateButtonLabel: "Запази Датата",
		setTimeButtonLabel: "Запази Часът",
		setDurationButtonLabel: "Запази Времето",
		calTodayButtonLabel: "Обратно до Днес",
		titleDateDialogLabel: "Избери Дата",
		titleTimeDialogLabel: "Избери Час",
		daysOfWeek: ["Неделя", "Понеделник", "Вторник", "Сряда", "Четвъртък", "Петък", "Събота"],
		daysOfWeekShort: ["Нд", "Пн", "Вт", "Ср", "Чт", "Пт", "Сб"],
		monthsOfYear: ["Януари", "Февруари", "Март", "Април", "Май", "Юни", "Юли", "Август", "Септември", "Октомври", "Ноември", "Декември"],
		monthsOfYearShort: ["Яну", "Фев", "Мар", "Апр", "Май", "Юни", "Юли", "Авг", "Сеп", "Окт", "Ное", "Дек"],
		durationLabel: ["Дни", "Часове", "Минути", "Секунди"],
		durationDays: ["Ден", "Дни"],
		tooltip: "Заредете Прозореца за Дата",
		nextMonth: "Следващ Месец",
		prevMonth: "Предишен Месец",
		timeFormat: 24,
		headerFormat: '%A, %B %-d, %Y',
		dateFieldOrder: ['d','m','y'],
		timeFieldOrder: ['h', 'i', 'a'],
		slideFieldOrder: ['y', 'm', 'd'],
		dateFormat: "%d.%-m.%Y",
		useArabicIndic: false,
		isRTL: false,
		calStartDay: 1,
		clearButton: "Изчисти",
		durationOrder: ['d', 'h', 'i', 's'],
		meridiem: ["AM", "PM"],
		timeOutput: "%l:%M %p",
		durationFormat: "%Dd %DA, %Dl:%DM:%DS",
		calDateListLabel: "Други дати",
		calHeaderFormat: "%B %Y"
	}
});
jQuery.extend(jQuery.mobile.datebox.prototype.options, {
	useLang: 'bg'
});

