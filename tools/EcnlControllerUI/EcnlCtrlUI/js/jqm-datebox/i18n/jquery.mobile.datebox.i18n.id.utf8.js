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
	'id': {
		setDateButtonLabel: "Atur Tanggal",
		setTimeButtonLabel: "Atur Waktu",
		setDurationButtonLabel: "Atur Durasi",
		calTodayButtonLabel: "Lompat ke hari ini",
		titleDateDialogLabel: "Pilih Tanggal",
		titleTimeDialogLabel: "Pilih Waktu",
		daysOfWeek: ["Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu"],
		daysOfWeekShort: ["Mi", "Se", "Se", "Ra", "Ka", "Jum", "Sab"],
		monthsOfYear: ["Januari", "Februari", "Maret", "April", "Mei", "Juni", "Juli", "Agustus", "September", "Oktober", "November", "Desember"],
		monthsOfYearShort: ["Jan", "Feb", "Mar", "Apr", "Me", "Jun", "Jul", "Agu", "Sep", "Okt", "Nov", "Des"],
		durationLabel: ["Hari", "Jam", "Menit", "Detik"],
		durationDays: ["Hari", "Hari"],
		tooltip: "Buka Date Picker",
		nextMonth: "Bulan Berikutnya",
		prevMonth: "Bulan Sebelumnya",
		timeFormat: 12,
		headerFormat: '%A, %B %-d, %Y',
		dateFieldOrder: ['d','m','y'],
		timeFieldOrder: ['h','i'],
		slideFieldOrder: ['d','m','y'],
		dateFormat: "%d-%m-%Y",
		useArabicIndic: false,
		isRTL: false,
		calStartDay: 1,
		clearButton: "Bersihkan",
		durationOrder: ['d','h','i','s'],
		meridiem: ["AM", "PM"],
		timeOutput: "%l:%M %p",
		durationFormat: "%Dd %DA, %Dl:%DM:%DS",
		calDateListLabel: "Tanggal lainnya",
		calHeaderFormat: "%B %Y"
	}
});
jQuery.extend(jQuery.mobile.datebox.prototype.options, {
	useLang: 'id'
});

