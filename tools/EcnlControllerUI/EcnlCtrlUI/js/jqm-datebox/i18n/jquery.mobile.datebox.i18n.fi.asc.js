/*
 * jQuery Mobile Framework : plugin to provide a date and time picker.
 * Copyright (c) JTSage
 * CC 3.0 Attribution.  May be relicensed without permission/notifcation.
 * https://github.com/jtsage/jquery-mobile-datebox
 *
 * Translation by: Ville Salonen <ville.salonen@iki.fi>
 *
 */

jQuery.extend(jQuery.mobile.datebox.prototype.options.lang, {
	'fi': {
		setDateButtonLabel: "Valitse p&#228;iv&#228;",
		setTimeButtonLabel: "Valitse aika",
		setDurationButtonLabel: "Valitse kesto",
		calTodayButtonLabel: "T&#228;n&#228;&#228;n",
		titleDateDialogLabel: "Valitse p&#228;iv&#228;m&#228;&#228;r&#228;",
		titleTimeDialogLabel: "Valitse aika",
		daysOfWeek: ["Sunnuntai", "Maanantai", "Tiistai", "Keskiviikko", "Torstai", "Perjantai", "Lauantai"],
		daysOfWeekShort: ["Su", "Ma", "Ti", "Ke", "To", "Pe", "La"],
		monthsOfYear: ["Tammikuu", "Helmikuu", "Maaliskuu", "Huhtikuu", "Toukokuu", "Kes&#228;kuu", "Hein&#228;kuu", "Elokuu", "Syyskuu", "Lokakuu", "Marraskuu", "Joulukuu"],
		monthsOfYearShort: ["Tammi", "Helmi", "Maali", "Huhti", "Touko", "Kes&#228;", "Hein&#228;", "Elo", "Syys", "Loka", "Marras", "Joulu"],
		durationLabel: ["P&#228;iv&#228;&#228;", "Tuntia", "Minuuttia", "Sekuntia"],
		durationDays: ["P&#228;iv&#228;", "P&#228;iv&#228;&#228;"],
		tooltip: "Avaa p&#228;iv&#228;m&#228;&#228;r&#228;valitsin",
		nextMonth: "Seuraava kuukausi",
		prevMonth: "Edellinen kuukausi",
		timeFormat: 24,
		headerFormat: '%A, %B %-d, %Y',
		dateFieldOrder: ['d','m','y'],
		timeFieldOrder: ['h', 'i', 'a'],
		slideFieldOrder: ['y', 'm', 'd'],
		dateFormat: "%d.%m.%Y",
		useArabicIndic: false,
		isRTL: false,
		calStartDay: 0,
		clearButton: "Selke&#228;&#228;",
		durationOrder: ['d', 'h', 'i', 's'],
		meridiem: ["AM", "PM"],
		timeOutput: "%k:%M",
		durationFormat: "%Dd %DA, %Dl:%DM:%DS",
		calDateListLabel: "Muut p&#228;iv&#228;m&#228;&#228;r&#228;t",
		calHeaderFormat: "%B %Y"
	}
});
jQuery.extend(jQuery.mobile.datebox.prototype.options, {
	useLang: 'fi'
});

