/*
 * jQuery Mobile Framework : plugin to provide a date and time picker.
 * Copyright (c) JTSage
 * CC 3.0 Attribution.  May be relicensed without permission/notifcation.
 * https://github.com/jtsage/jquery-mobile-datebox
 *
 * Translation by: Alexander Zolotko <azolotko@gmail.com>
 *
 */

jQuery.extend(jQuery.mobile.datebox.prototype.options.lang, {
	'uk': {
		setDateButtonLabel: "Встановити Дату",
		setTimeButtonLabel: "Встановити Час",
		setDurationButtonLabel: "Встановити Тривалість",
		calTodayButtonLabel: "Сьогодні",
		titleDateDialogLabel: "Вибрати дату",
		titleTimeDialogLabel: "Вибрати час",
		daysOfWeek: ["Неділя", "Понеділок", "Вівторок", "Середа", "Четвер", "П\'ятниця", "Субота"],
		daysOfWeekShort: ["Нд", "Пн", "Вт", "Ср", "Чт", "Пт", "Сб"],
		monthsOfYear: ["Січень", "Лютий", "Березень", "Квітень", "Травень", "Червень", "Липень", "Серпень", "Вересень", "Жовтень", "Листопад", "Грудень"],
		monthsOfYearShort: ["Січ", "Лют", "Бер", "Кві", "Тра", "Чер", "Лип", "Сер", "Веп", "Жов", "Лис", "Гру"],
		durationLabel: ["Дні", "Години", "Хвилини", "Секунди"],
		durationDays: ["День", "Дні"],
		tooltip: "Відкрити елемент вибору дати",
		nextMonth: "Наступного місяця",
		prevMonth: "Минулий місяць",
		timeFormat: 24,
		headerFormat: '%A, %B %-d, %Y',
		dateFieldOrder: ['d','m','y'],
		timeFieldOrder: ['h', 'i', 'a'],
		slideFieldOrder: ['y', 'm', 'd'],
		dateFormat: "%d.%m.%Y",
		useArabicIndic: false,
		isRTL: false,
		calStartDay: 0,
		clearButton: "Ясно",
		durationOrder: ['d', 'h', 'i', 's'],
		meridiem: ["AM", "PM"],
		timeOutput: "%k:%M",
		durationFormat: "%Dd %DA, %Dl:%DM:%DS",
		calDateListLabel: "Інші дати",
		calHeaderFormat: "%B %Y"
	}
});
jQuery.extend(jQuery.mobile.datebox.prototype.options, {
	useLang: 'uk'
});

