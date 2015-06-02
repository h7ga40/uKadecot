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
	'pt-PT': {
		setDateButtonLabel: "Fechar",
		setTimeButtonLabel: "Fechar",
		setDurationButtonLabel: "Fechar",
		calTodayButtonLabel: "Ir para hoje",
		titleDateDialogLabel: "Escolher data",
		titleTimeDialogLabel: "Escolha a hora",
		daysOfWeek: ["Domingo", "Segunda-feira", "Ter&#231;a-feira", "Quarta-feira", "Quinta-feira", "Sexta-feira", "S&#225;bado"],
		daysOfWeekShort: ["Dom", "Seg", "Ter", "Qua", "Qui", "Sex", "S&#225;b"],
		monthsOfYear: ["Janeiro", "Fevereiro", "Mar&#231;o", "Abril", "Maio", "Junho", "Julho", "Agosto", "Setembro", "Outubro", "Novembro", "Dezembro"],
		monthsOfYearShort: ["Jan", "Fev", "Mar", "Abr", "Mai", "Jun", "Jul", "Ago", "Set", "Out", "Nov", "Dez"],
		durationLabel: ["Dias", "Horas", "Minutos", "Segundos"],
		durationDays: ["Dia", "Dias"],
		tooltip: "Abrir selecionador de data",
		nextMonth: "M&#234;s Seguinte",
		prevMonth: "M&#234;s anterior",
		timeFormat: 24,
		headerFormat: '%A, %B %-d, %Y',
		dateFieldOrder: ['d','m','y'],
		timeFieldOrder: ['h', 'i', 'a'],
		slideFieldOrder: ['y', 'm', 'd'],
		dateFormat: "%d/%m/%y",
		useArabicIndic: false,
		isRTL: false,
		calStartDay: 0,
		clearButton: "Limpar",
		durationOrder: ['d', 'h', 'i', 's'],
		meridiem: ["AM", "PM"],
		timeOutput: "%k:%M",
		durationFormat: "%Dd %DA, %Dl:%DM:%DS",
		calDateListLabel: "Outras datas",
		calHeaderFormat: "%B %Y"
	}
});
jQuery.extend(jQuery.mobile.datebox.prototype.options, {
	useLang: 'pt-PT'
});

