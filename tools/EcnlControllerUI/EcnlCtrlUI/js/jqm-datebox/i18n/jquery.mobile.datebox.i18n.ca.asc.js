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
	'ca': {
		setDateButtonLabel: "Tanca",
		setTimeButtonLabel: "Tanca",
		setDurationButtonLabel: "Tanca",
		calTodayButtonLabel: "V&#233;s a avui",
		titleDateDialogLabel: "Tria la data",
		titleTimeDialogLabel: "Esculli temps",
		daysOfWeek: ["Diumenge", "Dilluns", "Dimarts", "Dimecres", "Dijous", "Divendres", "Dissabte"],
		daysOfWeekShort: ["Dg", "Dl", "Dt", "Dc", "Dj", "Dv", "Ds"],
		monthsOfYear: ["Gener", "Febrer", "Mar&#231;", "Abril", "Maig", "Juny", "Juliol", "Agost", "Setembre", "Octubre", "Novembre", "Desembre"],
		monthsOfYearShort: ["Gen", "Feb", "Mar&#231;", "Abr", "Maig", "Juny", "Jul", "Ag", "Set", "Oct", "Nov", "Des"],
		durationLabel: ["Dies", "Hores", "Minuts", "Segons"],
		durationDays: ["Dia", "Dies"],
		tooltip: "Selector de la data obert",
		nextMonth: "Seg&#252;ent",
		prevMonth: "Anterior",
		timeFormat: 24,
		headerFormat: '%A, %B %-d, %Y',
		dateFieldOrder: ['d','m','y'],
		timeFieldOrder: ['h', 'i', 'a'],
		slideFieldOrder: ['y', 'm', 'd'],
		dateFormat: "%d/%m/%y",
		useArabicIndic: false,
		isRTL: false,
		calStartDay: 0,
		clearButton: "Clar",
		durationOrder: ['d', 'h', 'i', 's'],
		meridiem: ["AM", "PM"],
		timeOutput: "%k:%M",
		durationFormat: "%Dd %DA, %Dl:%DM:%DS",
		calDateListLabel: "Altres Dates",
		calHeaderFormat: "%B %Y"
	}
});
jQuery.extend(jQuery.mobile.datebox.prototype.options, {
	useLang: 'ca'
});

