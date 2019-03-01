/*
 * The XML and API file generator module for SIP.
 *
 * Copyright (c) 2019 Riverbank Computing Limited <info@riverbankcomputing.com>
 *
 * This file is part of SIP.
 *
 * This copy of SIP is licensed for use under the terms of the SIP License
 * Agreement.  See the file LICENSE for more details.
 *
 * This copy of SIP may also used under the terms of the GNU General Public
 * License v2 or v3 as published by the Free Software Foundation which can be
 * found in the files LICENSE-GPL2 and LICENSE-GPL3 included in this package.
 *
 * SIP is supplied WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */


#include <stdio.h>
#include <string.h>

#include "sip.h"


#define XML_VERSION_NR  0       /* The schema version number. */

/* Icon numbers.  The values are those used by the eric IDE. */
#define CLASS_ID        1
#define METHOD_ID       4
#define VARIABLE_ID     7
#define ENUM_ID         10


static void apiEnums(sipSpec *pt, moduleDef *mod, classDef *scope, FILE *fp);
static void apiVars(sipSpec *pt, moduleDef *mod, classDef *scope, FILE *fp);
static void apiCtor(sipSpec *pt, moduleDef *mod, classDef *scope, ctorDef *ct,
        FILE *fp);
static void apiOverload(sipSpec *pt, moduleDef *mod, classDef *scope,
        overDef *od, FILE *fp);
static int apiArgument(sipSpec *pt, argDef *ad, int out, int need_comma,
        int names, int defaults, int in_str, FILE *fp);
static void xmlClass(sipSpec *pt, moduleDef *mod, classDef *cd, FILE *fp);
static void xmlEnums(sipSpec *pt, moduleDef *mod, classDef *scope, int indent,
        FILE *fp);
static void xmlVars(sipSpec *pt, moduleDef *mod, classDef *scope, int indent,
        FILE *fp);
static void xmlFunction(sipSpec *pt, moduleDef *mod, classDef *scope,
        memberDef *md, overDef *oloads, int indent, FILE *fp);
static void xmlCtor(sipSpec *pt, moduleDef *mod, classDef *scope, ctorDef *ct,
        int indent, FILE *fp);
static void xmlOverload(sipSpec *pt, moduleDef *mod, classDef *scope,
        memberDef *md, overDef *od, classDef *xtnds, int stat, int indent,
        FILE *fp);
static void xmlCppSignature(FILE *fp, signatureDef *sd, int is_const);
static void xmlArgument(sipSpec *pt, moduleDef *mod, argDef *ad, int out,
        KwArgs kwargs, int res_xfer, int indent, FILE *fp);
static void xmlType(sipSpec *pt, moduleDef *mod, argDef *ad, int out,
        KwArgs kwargs, FILE *fp);
static void xmlIndent(int indent, FILE *fp);
static void xmlRealName(scopedNameDef *fqcname, const char *member, FILE *fp);
static void xmlRealScopedName(classDef *scope, const char *cppname, FILE *fp);
static const char *pyType(sipSpec *pt, argDef *ad, classDef **scope);
static void exportPythonSignature(sipSpec *pt, FILE *fp, signatureDef *sd,
        int names, int defaults, int in_str, int is_signal);
static void restPyEnumMember(enumMemberDef *emd, FILE *fp);
static void restPyAttribute(moduleDef *mod, classDef *scope, nameDef *name,
        FILE *fp);
static int restValue(sipSpec *pt, valueDef *value, FILE *fp);
static const char *reflectedSlot(slotType st);
static int hasCppSignature(signatureDef *sd);


/*
 * Generate the API file.
 */
void generateAPI(sipSpec *pt, moduleDef *mod, const char *apiFile)
{
    overDef *od;
    classDef *cd;
    FILE *fp;

    /* Generate the file. */
    if ((fp = fopen(apiFile, "w")) == NULL)
        fatal("Unable to create file \"%s\"\n", apiFile);

    apiEnums(pt, mod, NULL, fp);
    apiVars(pt, mod, NULL, fp);

    for (od = mod->overs; od != NULL; od = od->next)
    {
        if (od->common->module != mod)
            continue;

        if (od->common->slot != no_slot)
            continue;

        apiOverload(pt, mod, NULL, od, fp);
    }

    for (cd = pt->classes; cd != NULL; cd = cd->next)
    {
        ctorDef *ct;

        if (cd->iff->module != mod)
            continue;

        if (isExternal(cd))
            continue;

        apiEnums(pt, mod, cd, fp);
        apiVars(pt, mod, cd, fp);

        for (ct = cd->ctors; ct != NULL; ct = ct->next)
        {
            if (isPrivateCtor(ct))
                continue;

            apiCtor(pt, mod, cd, ct, fp);
        }

        for (od = cd->overs; od != NULL; od = od->next)
        {
            if (isPrivate(od))
                continue;

            if (od->common->slot != no_slot)
                continue;

            apiOverload(pt, mod, cd, od, fp);
        }
    }

    fclose(fp);
}


/*
 * Generate an API ctor.
 */
static void apiCtor(sipSpec *pt, moduleDef *mod, classDef *scope, ctorDef *ct,
        FILE *fp)
{
    int need_comma, a;

    /* Do the callable type form. */
    fprintf(fp, "%s.", mod->name);
    prScopedPythonName(fp, scope->ecd, scope->pyname->text);
    fprintf(fp, "?%d(", CLASS_ID);

    need_comma = FALSE;

    for (a = 0; a < ct->pysig.nrArgs; ++a)
    {
        argDef *ad = &ct->pysig.args[a];

        need_comma = apiArgument(pt, ad, FALSE, need_comma, TRUE, TRUE, FALSE,
                fp);
    }

    fprintf(fp, ")\n");

    /* Do the call __init__ form. */
    fprintf(fp, "%s.", mod->name);
    prScopedPythonName(fp, scope->ecd, scope->pyname->text);
    fprintf(fp, ".__init__?%d(self", CLASS_ID);

    for (a = 0; a < ct->pysig.nrArgs; ++a)
        apiArgument(pt, &ct->pysig.args[a], FALSE, TRUE, TRUE, TRUE, FALSE,
                fp);

    fprintf(fp, ")\n");
}


/*
 * Generate the APIs for all the enums in a scope.
 */
static void apiEnums(sipSpec *pt, moduleDef *mod, classDef *scope, FILE *fp)
{
    enumDef *ed;

    for (ed = pt->enums; ed != NULL; ed = ed->next)
    {
        enumMemberDef *emd;

        if (ed->module != mod)
            continue;

        if (ed->ecd != scope)
            continue;

        if (ed->pyname != NULL)
        {
            fprintf(fp, "%s.", mod->name);
            prScopedPythonName(fp, ed->ecd, ed->pyname->text);
            fprintf(fp, "?%d\n", ENUM_ID);
        }

        for (emd = ed->members; emd != NULL; emd = emd->next)
        {
            fprintf(fp, "%s.", mod->name);
            prScopedPythonName(fp, ed->ecd, emd->pyname->text);
            fprintf(fp, "?%d\n", ENUM_ID);
        }
    }
}


/*
 * Generate the APIs for all the variables in a scope.
 */
static void apiVars(sipSpec *pt, moduleDef *mod, classDef *scope, FILE *fp)
{
    varDef *vd;

    for (vd = pt->vars; vd != NULL; vd = vd->next)
    {
        if (vd->module != mod)
            continue;

        if (vd->ecd != scope)
            continue;

        fprintf(fp, "%s.", mod->name);
        prScopedPythonName(fp, vd->ecd, vd->pyname->text);
        fprintf(fp, "?%d\n", VARIABLE_ID);
    }
}


/*
 * Generate a single API overload.
 */
static void apiOverload(sipSpec *pt, moduleDef *mod, classDef *scope,
        overDef *od, FILE *fp)
{
    fprintf(fp, "%s.", mod->name);
    prScopedPythonName(fp, scope, od->common->pyname->text);
    fprintf(fp, "?%d", METHOD_ID);

    exportPythonSignature(pt, fp, &od->pysig, TRUE, TRUE, FALSE, FALSE);

    fprintf(fp, "\n");
}


/*
 * Generate the API for an argument.
 */
static int apiArgument(sipSpec *pt, argDef *ad, int out, int need_comma,
        int names, int defaults, int in_str, FILE *fp)
{
    const char *tname;
    classDef *tscope;

    if (isArraySize(ad))
        return need_comma;

    if ((tname = pyType(pt, ad, &tscope)) == NULL)
        return need_comma;

    if (need_comma)
        fprintf(fp, ", ");

    prScopedPythonName(fp, tscope, tname);

    /*
     * Handle the default value is required, but ignore it if it is an output
     * only argument.
     */
    if (defaults && ad->defval && !out)
    {
        if (names && ad->name != NULL)
            fprintf(fp, " %s", ad->name->text);

        fprintf(fp, "=");
        prDefaultValue(ad, in_str, fp);
    }

    return TRUE;
}


/*
 * Generate the XML export file.
 */
void generateXML(sipSpec *pt, moduleDef *mod, const char *xmlFile)
{
    FILE *fp;
    classDef *cd;
    memberDef *md;

    if ((fp = fopen(xmlFile, "w")) == NULL)
        fatal("Unable to create file \"%s\"\n", xmlFile);

    fprintf(fp, "<?xml version=\"1.0\"?>\n");
    fprintf(fp, "<Module version=\"%u\" name=\"%s\">\n",
            XML_VERSION_NR, mod->name);

    /*
     * Note that we don't yet handle mapped types, templates or exceptions.
     */

    for (cd = pt->classes; cd != NULL; cd = cd->next)
    {
        if (cd->iff->module != mod)
            continue;

        if (isExternal(cd))
            continue;

        xmlClass(pt, mod, cd, fp);
    }

    for (cd = mod->proxies; cd != NULL; cd = cd->next)
        xmlClass(pt, mod, cd, fp);

    xmlEnums(pt, mod, NULL, 1, fp);
    xmlVars(pt, mod, NULL, 1, fp);

    for (md = mod->othfuncs; md != NULL; md = md->next)
        xmlFunction(pt, mod, NULL, md, mod->overs, 1, fp);

    fprintf(fp, "</Module>\n");

    fclose(fp);
}


/*
 * Generate a 'realname' attribute containing a fully qualified C/C++ name of
 * an object.
 */
static void xmlRealScopedName(classDef *scope, const char *cppname, FILE *fp)
{
    const char *sep = "";

    fprintf(fp, " realname=\"");

    if (scope != NULL)
    {
        scopedNameDef *snd;

        for (snd = removeGlobalScope(classFQCName(scope)); snd != NULL; snd = snd->next)
        {
            fprintf(fp, "%s%s", sep, snd->name);
            sep = "::";
        }
    }

    fprintf(fp, "%s%s\"", sep, cppname);
}


/*
 * Generate a 'realname' attribute containing a fully qualified C/C++ name.
 */
static void xmlRealName(scopedNameDef *fqcname, const char *member, FILE *fp)
{
    const char *sep = "";
    scopedNameDef *snd;

    fprintf(fp, " realname=\"");

    for (snd = removeGlobalScope(fqcname); snd != NULL; snd = snd->next)
    {
        fprintf(fp, "%s%s", sep, snd->name);
        sep = "::";
    }

    if (member != NULL)
        fprintf(fp, "::%s", member);

    fprintf(fp, "\"");
}


/*
 * Generate the XML for a class.
 */
static void xmlClass(sipSpec *pt, moduleDef *mod, classDef *cd, FILE *fp)
{
    int indent = 1;
    ctorDef *ct;
    memberDef *md;

    if (isOpaque(cd))
    {
        xmlIndent(indent, fp);
        fprintf(fp, "<OpaqueClass name=\"");
        prScopedPythonName(fp, cd->ecd, cd->pyname->text);
        fprintf(fp, "\"/>\n");

        return;
    }

    if (!isHiddenNamespace(cd))
    {
        xmlIndent(indent++, fp);
        fprintf(fp, "<Class name=\"");
        prScopedPythonName(fp, cd->ecd, cd->pyname->text);
        fprintf(fp, "\"");

        xmlRealName(classFQCName(cd), NULL, fp);

        if (cd->picklecode != NULL)
            fprintf(fp, " pickle=\"1\"");

        if (cd->convtocode != NULL)
            fprintf(fp, " convert=\"1\"");

        if (cd->convfromcode != NULL)
            fprintf(fp, " convertfrom=\"1\"");

        if (cd->real != NULL)
            fprintf(fp, " extends=\"%s\"", cd->real->iff->module->name);

        if (cd->pyqt_flags_enums != NULL)
        {
            const char *sep;
            stringList *sl;

            fprintf(fp, " flagsenums=\"");
            sep = "";

            for (sl = cd->pyqt_flags_enums; sl != NULL; sl = sl->next)
            {
                fprintf(fp, "%s%s", sep, sl->s);
                sep = " ";
            }

            fprintf(fp, "\"");
        }

        if (cd->supers != NULL)
        {
            classList *cl;

            fprintf(fp, " inherits=\"");

            for (cl = cd->supers; cl != NULL; cl = cl->next)
            {
                if (cl != cd->supers)
                    fprintf(fp, " ");

                restPyClass(cl->cd, fp);
            }

            fprintf(fp, "\"");
        }

        fprintf(fp, ">\n");
    }

    for (ct = cd->ctors; ct != NULL; ct = ct->next)
    {
        if (isPrivateCtor(ct))
            continue;

        xmlCtor(pt, mod, cd, ct, indent, fp);
    }

    xmlEnums(pt, mod, cd, indent, fp);
    xmlVars(pt, mod, cd, indent, fp);

    for (md = cd->members; md != NULL; md = md->next)
        xmlFunction(pt, mod, cd, md, cd->overs, indent, fp);

    if (!isHiddenNamespace(cd))
    {
        xmlIndent(--indent, fp);
        fprintf(fp, "</Class>\n");
    }
}


/*
 * Generate the XML for all the enums in a scope.
 */
static void xmlEnums(sipSpec *pt, moduleDef *mod, classDef *scope, int indent,
        FILE *fp)
{
    enumDef *ed;

    for (ed = pt->enums; ed != NULL; ed = ed->next)
    {
        if (ed->module != mod)
            continue;

        if (ed->ecd != scope)
            continue;

        if (ed->pyname != NULL)
        {
            enumMemberDef *emd;

            xmlIndent(indent++, fp);
            fprintf(fp, "<Enum name=\"");
            prScopedPythonName(fp, ed->ecd, ed->pyname->text);
            fprintf(fp, "\"");

            xmlRealName(ed->fqcname, NULL, fp);

            fprintf(fp, ">\n");

            for (emd = ed->members; emd != NULL; emd = emd->next)
            {
                xmlIndent(indent, fp);
                fprintf(fp, "<EnumMember name=\"");
                prScopedPythonName(fp, ed->ecd, ed->pyname->text);
                fprintf(fp, ".%s\"", emd->pyname->text);

                xmlRealName(ed->fqcname, emd->cname, fp);

                fprintf(fp, "/>\n");
            }

            xmlIndent(--indent, fp);
            fprintf(fp, "</Enum>\n");
        }
        else
        {
            enumMemberDef *emd;

            for (emd = ed->members; emd != NULL; emd = emd->next)
            {
                xmlIndent(indent, fp);
                fprintf(fp, "<Member name=\"");
                prScopedPythonName(fp, ed->ecd, emd->pyname->text);
                fprintf(fp, "\"");

                xmlRealScopedName(scope, emd->cname, fp);

                fprintf(fp, " const=\"1\" typename=\"int\"/>\n");
            }
        }
    }
}


/*
 * Generate the XML for all the variables in a scope.
 */
static void xmlVars(sipSpec *pt, moduleDef *mod, classDef *scope, int indent,
        FILE *fp)
{
    varDef *vd;

    for (vd = pt->vars; vd != NULL; vd = vd->next)
    {
        if (vd->module != mod)
            continue;

        if (vd->ecd != scope)
            continue;

        xmlIndent(indent, fp);
        fprintf(fp, "<Member name=\"");
        prScopedPythonName(fp, vd->ecd, vd->pyname->text);
        fprintf(fp, "\"");

        xmlRealName(vd->fqcname, NULL, fp);

        if (isConstArg(&vd->type) || scope == NULL)
            fprintf(fp, " const=\"1\"");

        if (isStaticVar(vd))
            fprintf(fp, " static=\"1\"");

        xmlType(pt, mod, &vd->type, FALSE, NoKwArgs, fp);
        fprintf(fp, "/>\n");
    }
}


/*
 * Generate the XML for a ctor.
 */
static void xmlCtor(sipSpec *pt, moduleDef *mod, classDef *scope, ctorDef *ct,
        int indent, FILE *fp)
{
    int a;

    xmlIndent(indent++, fp);
    fprintf(fp, "<Function name=\"");
    prScopedPythonName(fp, scope, "__init__");
    fprintf(fp, "\"");

    xmlRealScopedName(scope, "__init__", fp);

    if (hasCppSignature(ct->cppsig))
    {
        fprintf(fp, " cppsig=\"");
        xmlCppSignature(fp, ct->cppsig, FALSE);
        fprintf(fp, "\"");
    }

    /* Handle the trivial case. */
    if (ct->pysig.nrArgs == 0)
    {
        fprintf(fp, "/>\n");
        return;
    }

    fprintf(fp, ">\n");

    for (a = 0; a < ct->pysig.nrArgs; ++a)
    {
        argDef *ad = &ct->pysig.args[a];

        if (isInArg(ad))
            xmlArgument(pt, mod, ad, FALSE, ct->kwargs, FALSE, indent, fp);

        if (isOutArg(ad))
            xmlArgument(pt, mod, ad, TRUE, ct->kwargs, FALSE, indent, fp);
    }

    xmlIndent(--indent, fp);
    fprintf(fp, "</Function>\n");
}


/*
 * Generate the XML for a function.
 */
static void xmlFunction(sipSpec *pt, moduleDef *mod, classDef *scope,
        memberDef *md, overDef *oloads, int indent, FILE *fp)
{
    overDef *od;

    for (od = oloads; od != NULL; od = od->next)
    {
        int isstat;
        classDef *xtnds;

        if (od->common != md)
            continue;

        if (isPrivate(od))
            continue;

        if (isSignal(od))
        {
            int a;

            xmlIndent(indent++, fp);
            fprintf(fp, "<Signal name=\"");
            prScopedPythonName(fp, scope, md->pyname->text);
            fprintf(fp, "\"");

            xmlRealScopedName(scope, od->cppname, fp);

            if (hasCppSignature(od->cppsig))
            {
                fprintf(fp, " cppsig=\"");
                xmlCppSignature(fp, od->cppsig, FALSE);
                fprintf(fp, "\"");
            }

            /* Handle the trivial case. */
            if (od->pysig.nrArgs == 0)
            {
                fprintf(fp, "/>\n");
                continue;
            }

            fprintf(fp, ">\n");

            for (a = 0; a < od->pysig.nrArgs; ++a)
            {
                argDef *ad = &od->pysig.args[a];

                xmlArgument(pt, mod, ad, FALSE, od->kwargs, FALSE, indent, fp);
            }

            xmlIndent(--indent, fp);
            fprintf(fp, "</Signal>\n");

            continue;
        }

        xtnds = NULL;
        isstat = (scope == NULL || scope->iff->type == namespace_iface || isStatic(od));

        if (scope == NULL && md->slot != no_slot && od->pysig.args[0].atype == class_type)
        {
            xtnds = od->pysig.args[0].u.cd;
            isstat = FALSE;
        }

        xmlOverload(pt, mod, scope, md, od, xtnds, isstat, indent, fp);
    }
}


/*
 * Generate the XML for an overload.
 */
static void xmlOverload(sipSpec *pt, moduleDef *mod, classDef *scope,
        memberDef *md, overDef *od, classDef *xtnds, int stat, int indent,
        FILE *fp)
{
    const char *name, *cppname = od->cppname;
    int a, no_res;

    xmlIndent(indent++, fp);
    fprintf(fp, "<Function name=\"");

    if (isReflected(od))
    {
        if ((name = reflectedSlot(md->slot)) != NULL)
            cppname = name;
        else
            name = md->pyname->text;
    }
    else
    {
        name = md->pyname->text;
    }

    prScopedPythonName(fp, scope, name);

    fprintf(fp, "\"");

    xmlRealScopedName(scope, cppname, fp);

    if (hasCppSignature(od->cppsig))
    {
        fprintf(fp, " cppsig=\"");
        xmlCppSignature(fp, od->cppsig, isConst(od));
        fprintf(fp, "\"");
    }

    if (isAbstract(od))
        fprintf(fp, " abstract=\"1\"");

    if (stat)
        fprintf(fp, " static=\"1\"");

    if (isSlot(od))
        fprintf(fp, " slot=\"1\"");

    if (isVirtual(od))
    {
        fprintf(fp, " virtual=\"1\"");
    }

    if (xtnds != NULL)
    {
        fprintf(fp, " extends=\"");
        prScopedPythonName(fp, xtnds->ecd, xtnds->pyname->text);
        fprintf(fp, "\"");
    }

    /* An empty type hint specifies a void return. */
    if (od->pysig.result.typehint_out != NULL && od->pysig.result.typehint_out->raw_hint[0] == '\0')
        no_res = TRUE;
    else
        no_res = (od->pysig.result.atype == void_type && od->pysig.result.nrderefs == 0);

    /* Handle the trivial case. */
    if (no_res && od->pysig.nrArgs == 0)
    {
        fprintf(fp, "/>\n");
        return;
    }

    fprintf(fp, ">\n");

    if (!no_res)
        xmlArgument(pt, mod, &od->pysig.result, TRUE, NoKwArgs,
                isResultTransferredBack(od), indent, fp);

    for (a = 0; a < od->pysig.nrArgs; ++a)
    {
        argDef *ad = &od->pysig.args[a];

        /*
         * Ignore the first argument of non-reflected number slots and the
         * second argument of reflected number slots.
         */
        if (isNumberSlot(md) && od->pysig.nrArgs == 2)
            if ((a == 0 && !isReflected(od)) || (a == 1 && isReflected(od)))
                continue;

        if (isInArg(ad))
            xmlArgument(pt, mod, ad, FALSE, od->kwargs, FALSE, indent, fp);

        if (isOutArg(ad))
            xmlArgument(pt, mod, ad, TRUE, od->kwargs, FALSE, indent, fp);
    }

    xmlIndent(--indent, fp);
    fprintf(fp, "</Function>\n");
}


/*
 * Return TRUE if there is a C/C++ signature.
 */
static int hasCppSignature(signatureDef *sd)
{
    int a;

    if (sd == NULL)
        return FALSE;

    /*
     * See if there are any arguments that could only have come from
     * handwritten code.
     */
    for (a = 0; a < sd->nrArgs; ++a)
    {
        switch (sd->args[a].atype)
        {
        case pyobject_type:
        case pytuple_type:
        case pylist_type:
        case pydict_type:
        case pycallable_type:
        case pyslice_type:
        case pytype_type:
        case pybuffer_type:
        case capsule_type:
            return FALSE;

        default:
            break;
        }
    }

    return TRUE;
}


/*
 * Generate the XML for a C++ signature.
 */
static void xmlCppSignature(FILE *fp, signatureDef *sd, int is_const)
{
    int a;

    prcode(fp, "%M");
    normaliseArgs(sd);

    prcode(fp, "(");

    for (a = 0; a < sd->nrArgs; ++a)
    {
        argDef *ad = &sd->args[a];

        if (a > 0)
            prcode(fp, ",");

        generateBaseType(NULL, ad, TRUE, STRIP_GLOBAL, fp);
    }

    prcode(fp, ")%s", (is_const ? " const" : ""));

    restoreArgs(sd);
    prcode(fp, "%M");
}


/*
 * Generate the XML for an argument.
 */
static void xmlArgument(sipSpec *pt, moduleDef *mod, argDef *ad, int out,
        KwArgs kwargs, int res_xfer, int indent, FILE *fp)
{
    if (isArraySize(ad))
        return;

    xmlIndent(indent, fp);
    fprintf(fp, "<%s", (out ? "Return" : "Argument"));
    xmlType(pt, mod, ad, out, kwargs, fp);

    if (!out)
    {
        if (isAllowNone(ad))
            fprintf(fp, " allownone=\"1\"");

        if (isDisallowNone(ad))
            fprintf(fp, " disallownone=\"1\"");

        if (isTransferred(ad))
            fprintf(fp, " transfer=\"to\"");
        else if (isThisTransferred(ad))
            fprintf(fp, " transfer=\"this\"");
    }

    if (res_xfer || isTransferredBack(ad))
        fprintf(fp, " transfer=\"back\"");

    fprintf(fp, "/>\n");
}


/*
 * Generate the XML for a type.
 */
static void xmlType(sipSpec *pt, moduleDef *mod, argDef *ad, int out,
        KwArgs kwargs, FILE *fp)
{
    const char *type_name;
    classDef *type_scope;
    typeHintDef *thd;

    fprintf(fp, " typename=\"");

    /* Handle the argument name. */
    if (!out && ad->name != NULL)
    {
        if (kwargs == AllKwArgs || (kwargs == OptionalKwArgs && ad->defval != NULL))
            fprintf(fp, "%s: ", ad->name->text);
    }

    /* Use any explicit type hint unless the argument is constrained. */
    thd = (out ? ad->typehint_out : (isConstrained(ad) ? NULL : ad->typehint_in));

    if (thd != NULL)
    {
        pyiTypeHint(pt, thd, mod, out, NULL, FALSE, TRUE, fp);
    }
    else
    {
        switch (ad->atype)
        {
        case class_type:
            restPyClass(ad->u.cd, fp);
            break;

        case enum_type:
            if (ad->u.ed->pyname != NULL)
                restPyEnum(ad->u.ed, fp);
            else
                fprintf(fp, "int");

            break;

        case qobject_type:
            restPyClass(pt->qobject_cd, fp);
            break;

        case mapped_type:
            /* There would normally be a type hint. */
            fprintf(fp, "unknown-type");
            break;

        default:
            if ((type_name = pyType(pt, ad, &type_scope)) != NULL)
                prScopedPythonName(fp, type_scope, type_name);
        }
    }

    if (!out && ad->name != NULL && ad->defval != NULL)
    {
        fprintf(fp, " = ");

        /*
         * Try and convert the value to a reST reference.  We don't try very
         * hard but will get most cases.
         */
        if (!restValue(pt, ad->defval, fp))
            prDefaultValue(ad, FALSE, fp);
    }

    fprintf(fp, "\"");
}


/*
 * Generate the indentation for a line.
 */
static void xmlIndent(int indent, FILE *fp)
{
    while (indent-- > 0)
        fprintf(fp, "  ");
}


/*
 * Get the Python representation of a type.
 */
static const char *pyType(sipSpec *pt, argDef *ad, classDef **scope)
{
    const char *type_name;

    *scope = NULL;

    /* Use any explicit documented type. */
    if (ad->doctype != NULL)
        return ad->doctype;

    /* For classes and mapped types we need the default implementation. */
    if (ad->atype == class_type || ad->atype == mapped_type)
    {
        classDef *def_cd = NULL;
        mappedTypeDef *def_mtd = NULL;
        ifaceFileDef *iff;

        if (ad->atype == class_type)
        {
            iff = ad->u.cd->iff;

            if (iff->api_range == NULL)
            {
                /* There is only one implementation. */
                def_cd = ad->u.cd;
                iff = NULL;
            }
        }
        else
        {
            iff = ad->u.mtd->iff;

            if (iff->api_range == NULL)
            {
                /* There is only one implementation. */
                def_mtd = ad->u.mtd;
                iff = NULL;
            }
        }

        if (iff != NULL)
        {
            int def_api;

            /* Find the default implementation. */
            def_api = findAPI(pt, iff->api_range->api_name->text)->from;

            for (iff = iff->first_alt; iff != NULL; iff = iff->next_alt)
            {
                apiVersionRangeDef *avd = iff->api_range;

                if (avd->from > 0 && avd->from > def_api)
                    continue;

                if (avd->to > 0 && avd->to <= def_api)
                    continue;

                /* It's within range. */
                break;
            }

            /* Find the corresponding class or mapped type. */
            for (def_cd = pt->classes; def_cd != NULL; def_cd = def_cd->next)
                if (def_cd->iff == iff)
                    break;

            if (def_cd == NULL)
                for (def_mtd = pt->mappedtypes; def_mtd != NULL; def_mtd = def_mtd->next)
                    if (def_mtd->iff == iff)
                        break;
        }

        /* Now handle the correct implementation. */
        if (def_cd != NULL)
        {
            *scope = def_cd->ecd;
            type_name = def_cd->pyname->text;
        }
        else
        {
            /*
             * Give a hint that /DocType/ should be used, or there is no
             * default implementation.
             */
            type_name = "unknown-type";

            if (def_mtd != NULL)
            {
                if (def_mtd->doctype != NULL)
                    type_name = def_mtd->doctype;
                else if (def_mtd->pyname != NULL)
                    type_name = def_mtd->pyname->text;
            }
        }

        return type_name;
    }

    switch (ad->atype)
    {
    case capsule_type:
        type_name = scopedNameTail(ad->u.cap);
        break;

    case struct_type:
    case void_type:
        type_name = "sip.voidptr";
        break;

    case enum_type:
        if (ad->u.ed->pyname != NULL)
        {
            type_name = ad->u.ed->pyname->text;
            *scope = ad->u.ed->ecd;
        }
        else
            type_name = "int";
        break;

    case qobject_type:
        type_name = "QObject";
        break;

    case ustring_type:
        /* Correct for Python v3. */
        type_name = "bytes";
        break;

    case string_type:
    case sstring_type:
    case wstring_type:
    case ascii_string_type:
    case latin1_string_type:
    case utf8_string_type:
        type_name = isArray(ad) ? "bytes" : "str";
        break;

    case byte_type:
    case sbyte_type:
    case ubyte_type:
    case ushort_type:
    case uint_type:
    case long_type:
    case longlong_type:
    case ulong_type:
    case ulonglong_type:
    case short_type:
    case int_type:
    case cint_type:
    case ssize_type:
    case size_type:
        type_name = "int";
        break;

    case float_type:
    case cfloat_type:
    case double_type:
    case cdouble_type:
        type_name = "float";
        break;

    case bool_type:
    case cbool_type:
        type_name = "bool";
        break;

    case pyobject_type:
        type_name = "object";
        break;

    case pytuple_type:
        type_name = "tuple";
        break;

    case pylist_type:
        type_name = "list";
        break;

    case pydict_type:
        type_name = "dict";
        break;

    case pycallable_type:
        type_name = "callable";
        break;

    case pyslice_type:
        type_name = "slice";
        break;

    case pytype_type:
        type_name = "type";
        break;

    case pybuffer_type:
        type_name = "buffer";
        break;

    case ellipsis_type:
        type_name = "...";
        break;

    default:
        type_name = NULL;
    }

    return type_name;
}


/*
 * Generate a Python signature.
 */
static void exportPythonSignature(sipSpec *pt, FILE *fp, signatureDef *sd,
        int names, int defaults, int in_str, int is_signal)
{
    int need_comma = FALSE, is_res, nr_out, a;

    if (is_signal)
    {
        if (sd->nrArgs != 0)
            fprintf(fp, "[");
    }
    else
    {
        fprintf(fp, "(");
    }

    nr_out = 0;

    for (a = 0; a < sd->nrArgs; ++a)
    {
        argDef *ad = &sd->args[a];

        if (isOutArg(ad))
            ++nr_out;

        if (!isInArg(ad))
            continue;

        need_comma = apiArgument(pt, ad, FALSE, need_comma, names, defaults,
                in_str, fp);
    }

    if (is_signal)
    {
        if (sd->nrArgs != 0)
            fprintf(fp, "]");
    }
    else
    {
        fprintf(fp, ")");
    }


    is_res = !((sd->result.atype == void_type && sd->result.nrderefs == 0) ||
            (sd->result.doctype != NULL && sd->result.doctype[0] == '\0'));

    if (is_res || nr_out > 0)
    {
        fprintf(fp, " -> ");

        if ((is_res && nr_out > 0) || nr_out > 1)
            fprintf(fp, "(");

        if (is_res)
            need_comma = apiArgument(pt, &sd->result, TRUE, FALSE, FALSE,
                    FALSE, in_str, fp);
        else
            need_comma = FALSE;

        for (a = 0; a < sd->nrArgs; ++a)
        {
            argDef *ad = &sd->args[a];

            if (isOutArg(ad))
                /* We don't want the name in the result tuple. */
                need_comma = apiArgument(pt, ad, TRUE, need_comma, FALSE,
                        FALSE, in_str, fp);
        }

        if ((is_res && nr_out > 0) || nr_out > 1)
            fprintf(fp, ")");
    }
}


/*
 * Generate a fully qualified class name as a reST reference.
 */
void restPyClass(classDef *cd, FILE *fp)
{
    fprintf(fp, ":sip:ref:`~%s.", cd->iff->module->fullname->text);
    prScopedPythonName(fp, cd->ecd, cd->pyname->text);
    fprintf(fp, "`");
}


/*
 * Generate a fully qualified enum name as a reST reference.
 */
void restPyEnum(enumDef *ed, FILE *fp)
{
    fprintf(fp, ":sip:ref:`~%s.", ed->module->fullname->text);
    prScopedPythonName(fp, ed->ecd, ed->pyname->text);
    fprintf(fp, "`");
}


/*
 * Generate a fully qualified attribute name as a reST reference.
 */
static void restPyEnumMember(enumMemberDef *emd, FILE *fp)
{
    fprintf(fp, ":sip:ref:`~%s.", emd->ed->module->fullname->text);
    prScopedPythonName(fp, emd->ed->ecd, emd->ed->pyname->text);
    fprintf(fp, ".%s`", emd->pyname->text);
}


/*
 * Generate a fully qualified attribute name as a reST reference.
 */
static void restPyAttribute(moduleDef *mod, classDef *scope, nameDef *name,
        FILE *fp)
{
    fprintf(fp, ":sip:ref:`~%s.", mod->fullname->text);
    prScopedPythonName(fp, scope, name->text);
    fprintf(fp, "`");
}


/*
 * Generate a reST reference for a scoped name if possible.  Return TRUE if
 * something was generated.
 */
static int restValue(sipSpec *pt, valueDef *value, FILE *fp)
{
    const char *name;
    scopedNameDef *target, *scope, *snd;
    varDef *vd;
    enumDef *ed;

    /* The value must be a scoped name and we don't handle expressions. */
    if (value->vtype != scoped_value || value->next != NULL)
        return FALSE;

    target = value->u.vscp;

    /* See if it is an attribute. */
    for (vd = pt->vars; vd != NULL; vd = vd->next)
        if (compareScopedNames(vd->fqcname, target) == 0)
        {
            restPyAttribute(vd->module, vd->ecd, vd->pyname, fp);

            return TRUE;
        }

    /* Get the name and scope. */
    name = scopedNameTail(target);

    scope = NULL;

    for (snd = target; snd->name != name; snd = snd->next)
        appendScopedName(&scope, text2scopePart(snd->name));

    /* See if it is an enum member. */
    for (ed = pt->enums; ed != NULL; ed = ed->next)
    {
        enumMemberDef *emd;

        /*
         * Look for the member name first before working out if it is the
         * correct enum.
         */
        for (emd = ed->members; emd != NULL; emd = emd->next)
        {
            if (strcmp(emd->cname, name) == 0)
            {
                if (isScopedEnum(ed))
                {
                    /*
                     * It's a scoped enum so the fully qualified name of the
                     * enum must match the scope of the name.
                     */
                    if (scope != NULL && compareScopedNames(ed->fqcname, scope) == 0)
                    {
                        restPyEnumMember(emd, fp);

                        freeScopedName(scope);

                        return TRUE;
                    }
                }
                else
                {
                    /*
                     * It's a traditional enum so the scope of the enum must
                     * match the scope of the name.
                     */
                    if ((ed->ecd == NULL && scope == NULL) || (ed->ecd != NULL && scope != NULL && compareScopedNames(ed->ecd->iff->fqcname, scope) == 0))
                    {
                        if (ed->fqcname == NULL)
                            restPyAttribute(ed->module, ed->ecd, emd->pyname,
                                    fp);
                        else
                            restPyEnumMember(emd, fp);

                        freeScopedName(scope);

                        return TRUE;
                    }
                }

                break;
            }
        }
    }

    freeScopedName(scope);

    return FALSE;
}


/*
 * Return the name of the reflected version of a slot or NULL if it doesn't
 * have one.
 */
static const char *reflectedSlot(slotType st)
{
    switch (st)
    {
    case add_slot:
        return "__radd__";

    case sub_slot:
        return "__rsub__";

    case mul_slot:
        return "__rmul__";

    case matmul_slot:
        return "__rmatmul__";

    case truediv_slot:
        return "__rtruediv__";

    case floordiv_slot:
        return "__rfloordiv__";

    case mod_slot:
        return "__rmod__";

    case lshift_slot:
        return "__rlshift__";

    case rshift_slot:
        return "__rrshift__";

    case and_slot:
        return "__rand__";

    case or_slot:
        return "__ror__";

    case xor_slot:
        return "__rxor__";

    default:
        break;
    }

    return NULL;
}
