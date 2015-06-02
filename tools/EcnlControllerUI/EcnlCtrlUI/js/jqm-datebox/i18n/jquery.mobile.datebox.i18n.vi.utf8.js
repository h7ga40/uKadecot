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
	'vi': {
		setDateButtonLabel: "Đóng",
		setTimeButtonLabel: "Đóng",
		setDurationButtonLabel: "Đóng",
		calTodayButtonLabel: "Hôm nay",
		titleDateDialogLabel: "Chọn ngày",
		titleTimeDialogLabel: "Chọn thời gian",
		daysOfWeek: ["Chủ Nhật", "Thứ hai", "Thứ Ba", "Thứ Tư", "Thứ Năm", "Thứ sáu", "Thứ Bảy"],
		daysOfWeekShort: ["CN", "T2", "T3", "T4", "T5", "T6", "T7"],
		monthsOfYear: ["Tháng một", "Tháng hai", "Tháng ba", "Tháng tư", "Tháng năm", "Tháng Sáu", "Tháng Bảy", "Tháng Tám", "Tháng Chín", "Tháng Mười", "Tháng mười một", "Tháng mười hai"],
		monthsOfYearShort: ["Tháng một", "Tháng hai", "Tháng ba", "Tháng tư", "Tháng năm", "Tháng Sáu", "Tháng Bảy", "Tháng Tám", "Tháng Chín", "Tháng Mười", "Tháng mười một", "Tháng mười hai"],
		durationLabel: ["Ngày", "Giờ", "Phút", "Giây"],
		durationDays: ["Ngày", "Ngày"],
		tooltip: "Mở bảng chọn ngày",
		nextMonth: "Tiếp",
		prevMonth: "Trước",
		timeFormat: 24,
		headerFormat: '%A, %B %-d, %Y',
		dateFieldOrder: ['d','m','y'],
		timeFieldOrder: ['h', 'i', 'a'],
		slideFieldOrder: ['y', 'm', 'd'],
		dateFormat: "%d/%m/%Y",
		useArabicIndic: false,
		isRTL: false,
		calStartDay: 0,
		clearButton: "Rõ ràng",
		durationOrder: ['d', 'h', 'i', 's'],
		meridiem: ["AM", "PM"],
		timeOutput: "%k:%M",
		durationFormat: "%Dd %DA, %Dl:%DM:%DS",
		calDateListLabel: "Ngày khác",
		calHeaderFormat: "%B %Y"
	}
});
jQuery.extend(jQuery.mobile.datebox.prototype.options, {
	useLang: 'vi'
});

