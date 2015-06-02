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
	'sr': {
		setDateButtonLabel: "Затвори",
		setTimeButtonLabel: "Затвори",
		setDurationButtonLabel: "Затвори",
		calTodayButtonLabel: "Данас",
		titleDateDialogLabel: "Изаберите датум",
		titleTimeDialogLabel: "Одаберите време",
		daysOfWeek: ["Недеља", "Понедељак", "Уторак", "Среда", "Четвртак", "Петак", "Субота"],
		daysOfWeekShort: ["Нед", "Пон", "Уто", "Сре", "Чет", "Пет", "Суб"],
		monthsOfYear: ["Јануар", "Фебруар", "Март", "Април", "Мај", "Јун", "Јул", "Август", "Септембар", "Октобар", "Новембар", "Децембар"],
		monthsOfYearShort: ["Јан", "Феб", "Мар", "Апр", "Мај", "Јун", "Јул", "Авг", "Сеп", "Окт", "Нов", "Дец"],
		durationLabel: ["дана", "сат", "минута", "секунди"],
		durationDays: ["дан", "дана"],
		tooltip: "Отворите Цонтрол",
		nextMonth: "следећи",
		prevMonth: "Претходна",
		timeFormat: 24,
		headerFormat: '%A, %B %-d, %Y',
		dateFieldOrder: ['d','m','y'],
		timeFieldOrder: ['h', 'i', 'a'],
		slideFieldOrder: ['y', 'm', 'd'],
		dateFormat: "%d/%m/%Y",
		useArabicIndic: false,
		isRTL: false,
		calStartDay: 0,
		clearButton: "Обриши",
		durationOrder: ['d', 'h', 'i', 's'],
		meridiem: ["AM", "PM"],
		timeOutput: "%k:%M",
		durationFormat: "%Dd %DA, %Dl:%DM:%DS",
		calDateListLabel: "Остали датуми",
		calHeaderFormat: "%B %Y"
	}
});
jQuery.extend(jQuery.mobile.datebox.prototype.options, {
	useLang: 'sr'
});

