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
	'fr': {
		setDateButtonLabel: "Date Fix&#233;e",
		setTimeButtonLabel: "R&#233;gler l'heure",
		setDurationButtonLabel: "R&#233;gler la dur&#233;e",
		calTodayButtonLabel: "Aller &#224; aujourd'hui",
		titleDateDialogLabel: "Choisir la Date",
		titleTimeDialogLabel: "Choisir le temps",
		daysOfWeek: ["Dimanche", "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi"],
		daysOfWeekShort: ["D", "L", "M", "M", "J", "V", "S"],
		monthsOfYear: ["Janvier", "F&#233;vrier", "Mars", "Avril", "Mai", "Juin", "Juillet", "Ao&#251;t", "Septembre", "Octobre", "Novembre", "D&#233;cembre"],
		monthsOfYearShort: ["Janv", "F&#233;vr", "Mars", "Avr", "Mai", "Juin", "Juil", "Ao&#251;t", "Sept", "Oct", "Nov", "D&#233;c"],
		durationLabel: ["Jours", "Heures", "Minutes", "Secondes"],
		durationDays: ["Jour", "Jours"],
		tooltip: "Ouvrir le s&#233;lecteur de date",
		nextMonth: "Mois Suiv.",
		prevMonth: "Mois Prec.",
		timeFormat: 24,
		headerFormat: '%A, %-d %B, %Y',
		dateFieldOrder: ['d','m','y'],
		timeFieldOrder: ['h', 'i', 'a'],
		slideFieldOrder: ['y', 'm', 'd'],
		dateFormat: "%d/%m/%Y",
		useArabicIndic: false,
		isRTL: false,
		calStartDay: 1,
		clearButton: "Effacer",
		durationOrder: ['d', 'h', 'i', 's'],
		meridiem: ["AM", "PM"],
		timeOutput: "%k:%M",
		durationFormat: "%Dd %DA, %Dl:%DM:%DS",
		calDateListLabel: "Autres Dates",
		calHeaderFormat: "%B %Y"
	}
});
jQuery.extend(jQuery.mobile.datebox.prototype.options, {
	useLang: 'fr'
});

