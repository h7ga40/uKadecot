/*
 * jQuery Mobile Framework : plugin to provide a date and time picker.
 * Copyright (c) JTSage
 * CC 3.0 Attribution.  May be relicensed without permission/notifcation.
 * https://github.com/jtsage/jquery-mobile-datebox
 *
 * Translation by: Rodrigo Vieira <rodrigovieira1994@gmail.com>, Marcelo Dezem <crowdin>
 *
 */

jQuery.extend(jQuery.mobile.datebox.prototype.options.lang, {
	'pt-BR': {
		setDateButtonLabel: "Informar data",
		setTimeButtonLabel: "Informar hora",
		setDurationButtonLabel: "Informar duração",
		calTodayButtonLabel: "Ir para hoje",
		titleDateDialogLabel: "Escolher data",
		titleTimeDialogLabel: "Escolha a hora",
		daysOfWeek: ["Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sábado"],
		daysOfWeekShort: ["D", "S", "T", "Q", "Q", "S", "S"],
		monthsOfYear: ["Janeiro", "Fevereiro", "Março", "Abril", "Maio", "Junho", "Julho", "Agosto", "Setembro", "Outubro", "Novembro", "Dezembro"],
		monthsOfYearShort: ["Jan", "Fev", "Mar", "Abr", "Mai", "Jun", "Jul", "Ago", "Set", "Out", "Nov", "Dez"],
		durationLabel: ["Dias", "Horas", "Minutos", "Segundos"],
		durationDays: ["Dia", "Dias"],
		tooltip: "Selecionador de data aberta",
		nextMonth: "Próximo mês",
		prevMonth: "Mês anterior",
		timeFormat: 24,
		headerFormat: '%A, %B %-d, %Y',
		dateFieldOrder: ['d','m','y'],
		timeFieldOrder: ['h', 'i', 'a'],
		slideFieldOrder: ['y', 'm', 'd'],
		dateFormat: "%d/%m/%Y",
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
	useLang: 'pt-BR'
});

