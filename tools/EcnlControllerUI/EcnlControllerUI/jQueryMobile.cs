/*
 *  TOPPERS ECHONET Lite Communication Middleware
 * 
 *  Copyright (C) 2015 Cores Co., Ltd. Japan
 * 
 *  上記著作権者は，以下の(1)～(4)の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *      の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *      と．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *        報告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *      免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *  の責任を負わない．
 * 
 *  @(#) $Id$
 */

using System;
using System.Collections;
using System.Collections.Generic;
using System.Html;
using System.Text;
using jQueryApi;

public static class jQueryMobile
{
    [System.Runtime.CompilerServices.InlineCode("{obj}.collapsibleset({})")]
    public static jQueryObject CollapsibleSet(this jQueryObject obj)
    {
        return ((dynamic)obj).collapsibleset();
    }

    [System.Runtime.CompilerServices.InlineCode("{obj}.collapsibleset({p})")]
    public static void CollapsibleSet(this jQueryObject obj, string p)
    {
        ((dynamic)obj).collapsibleset(p);
    }

    [System.Runtime.CompilerServices.InlineCode("{obj}.popup({p1}, {p2})")]
    public static void Popup(this jQueryObject obj, string p1, object p2)
    {
        ((dynamic)obj).popup(p1, p2);
    }

    [System.Runtime.CompilerServices.InlineCode("{obj}.listview()")]
    public static jQueryObject ListView(this jQueryObject obj)
    {
        return ((dynamic)obj).listview();
    }

    [System.Runtime.CompilerServices.InlineCode("{obj}.listview({p})")]
    public static void ListView(this jQueryObject obj, string p)
    {
        ((dynamic)obj).listview(p);
    }

    [System.Runtime.CompilerServices.InlineCode("{obj}.tabs({p1}, {p2})")]
    public static void Tabs(this jQueryObject obj, string p1, int p2)
    {
        ((dynamic)obj).tabs(p1, p2);
    }

    [System.Runtime.CompilerServices.InlineCode("{obj}.tabs({p1}, {p2}, {p3})")]
    public static void Tabs(this jQueryObject obj, string p1, string p2, int p3)
    {
        ((dynamic)obj).tabs(p1, p2, p3);
    }

    [System.Runtime.CompilerServices.InlineCode("{obj}.checkboxradio()")]
    public static jQueryObject CheckboxRadio(this jQueryObject obj)
    {
        return ((dynamic)obj).checkboxradio();
    }

    [System.Runtime.CompilerServices.InlineCode("{obj}.checkboxradio({p})")]
    public static void CheckboxRadio(this jQueryObject obj, string p)
    {
        ((dynamic)obj).checkboxradio(p);
    }

    [System.Runtime.CompilerServices.InlineCode("{obj}.collapsible({p})")]
    public static void Collapsible(this jQueryObject obj, string p)
    {
        ((dynamic)obj).collapsible(p);
    }

    [System.Runtime.CompilerServices.InlineCode("{obj}.button({p1}, {p2}, {p3})")]
    public static void Button(this jQueryObject obj, string p1, string p2, object p3)
    {
        ((dynamic)obj).button(p1, p2, p3);
    }

    [System.Runtime.CompilerServices.InlineCode("{obj}.controlgroup()")]
    public static jQueryObject ControlGroup(this jQueryObject obj)
    {
        return ((dynamic)obj).controlgroup();
    }

    [System.Runtime.CompilerServices.InlineCode("{obj}.controlgroup({p})")]
    public static void ControlGroup(this jQueryObject obj, string p)
    {
        ((dynamic)obj).controlgroup(p);
    }

    [System.Runtime.CompilerServices.InlineCode("{obj}.textinput()")]
    public static jQueryObject TextInput(this jQueryObject obj)
    {
        return ((dynamic)obj).textinput();
    }

    [System.Runtime.CompilerServices.InlineCode("{obj}.textinput({p})")]
    public static void TextInput(this jQueryObject obj, string p)
    {
        ((dynamic)obj).textinput(p);
    }

    [System.Runtime.CompilerServices.InlineCode("{obj}.slider()")]
    public static jQueryObject Slider(this jQueryObject obj)
    {
        return ((dynamic)obj).slider();
    }

    [System.Runtime.CompilerServices.InlineCode("{obj}.slider({p})")]
    public static void Slider(this jQueryObject obj, string p)
    {
        ((dynamic)obj).slider(p);
    }

    [System.Runtime.CompilerServices.InlineCode("{obj}.flipswitch()")]
    public static jQueryObject FlipSwitch(this jQueryObject obj)
    {
        return ((dynamic)obj).flipswitch();
    }

    [System.Runtime.CompilerServices.InlineCode("{obj}.flipswitch({p})")]
    public static void FlipSwitch(this jQueryObject obj, string p)
    {
        ((dynamic)obj).flipswitch(p);
    }

    [System.Runtime.CompilerServices.InlineCode("{obj}.flipswitch({p1},{p2},{p3})")]
    public static void FlipSwitch(this jQueryObject obj, string p1, string p2, string p3)
    {
        ((dynamic)obj).flipswitch(p1, p2, p3);
    }
    [System.Runtime.CompilerServices.InlineCode("$.mobile.loading({p1})")]
    public static void Loading(string p1)
    {

    }
    [System.Runtime.CompilerServices.InlineCode("$.mobile.loading({p1},{p2})")]
    public static void Loading(string p1, JsDictionary p2)
    {

    }
    [System.Runtime.CompilerServices.InlineCode("$.mobile.loading({p1},{p2})")]
    public static void Loading(string p1, LoadingOption p2)
    {

    }

    [System.Runtime.CompilerServices.InlineCode("{obj}.prop({p1},{p2})")]
    public static jQueryObject Prop(this jQueryObject obj, string p1, bool p2)
    {
        return ((dynamic)obj).prop(p1, p2);
    }
    [System.Runtime.CompilerServices.InlineCode("{obj}.prop({p})")]
    public static bool Prop(this jQueryObject obj, string p)
    {
        return ((dynamic)obj).prop(p);
    }
}

public static class ElementExtensions
{
    [System.Runtime.CompilerServices.InlineCode("{obj}.value")]
    public static string GetValue(this Element obj)
    {
        return ((dynamic)obj).value;
    }

    [System.Runtime.CompilerServices.InlineCode("{obj}.value = {value}")]
    public static void SetValue(this Element obj, string value)
    {
        ((dynamic)obj).value = value;
    }

    [System.Runtime.CompilerServices.InlineCode("{obj}.checked")]
    public static bool GetChecked(this Element obj)
    {
        return ((dynamic)obj).@checked;
    }

    [System.Runtime.CompilerServices.InlineCode("{obj}.checked = {checked}")]
    public static void SetChecked(this Element obj, bool @checked)
    {
        ((dynamic)obj).@checked = @checked;
    }

    [System.Runtime.CompilerServices.InlineCode("{obj}.disabled")]
    public static bool GetDisabled(this Element obj)
    {
        return ((dynamic)obj).disabled;
    }

    [System.Runtime.CompilerServices.InlineCode("{obj}.disabled = {disabled}")]
    public static void SetDisabled(this Element obj, bool disabled)
    {
        ((dynamic)obj).disabled = disabled;
    }
}
public class LoadingOption
{
    public LoadingOption()
    { }
    public LoadingOption(string text , bool textVisible)
    {
        this.text = text;
        this.textVisible = textVisible;
    }
    public string text;
    public bool textVisible;
}