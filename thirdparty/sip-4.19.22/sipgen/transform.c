/*
 * The parse tree transformation module for SIP.
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


#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#include "sip.h"


static int samePythonSignature(signatureDef *sd1, signatureDef *sd2);
static int nextSignificantArg(signatureDef *sd, int a);
static int sameArgType(argDef *a1, argDef *a2, int strict);
static int supportedType(classDef *,overDef *,argDef *,int);
static int sameCppOverload(overDef *od1, overDef *od2);
static void setAllImports(moduleDef *mod);
static void addUniqueModule(moduleDef *mod, moduleDef *imp);
static void ensureInput(classDef *,overDef *,argDef *);
static void defaultInput(argDef *);
static void defaultOutput(argDef *ad);
static void createSortedNumberedTypesTable(sipSpec *pt, moduleDef *mod);
static int compareTypes(const void *t1, const void *t2);
static void addAutoOverload(sipSpec *,classDef *,overDef *);
static void ifaceFileIsUsed(ifaceFileList **used, argDef *ad, int need_types);
static void ifaceFilesAreUsedByOverload(ifaceFileList **used, overDef *od,
        int need_types);
static void ifaceFilesAreUsedBySignature(ifaceFileList **used,
        signatureDef *sd, int need_types);
static void scopeDefaultValue(sipSpec *,classDef *,argDef *);
static void setHierarchy(sipSpec *,classDef *,classDef *,classList **);
static void transformModules(sipSpec *pt, int strict, moduleDef *mod);
static void transformCtors(sipSpec *,classDef *);
static void transformCasts(sipSpec *,classDef *);
static void addDefaultCopyCtor(classDef *);
static void transformScopeOverloads(sipSpec *pt, int strict, classDef *c_scope,
        mappedTypeDef *mt_scope, overDef *overs);
static void transformVariableList(sipSpec *pt, moduleDef *mod);
static void transformMappedTypes(sipSpec *pt, int strict, moduleDef *mod);
static void getVisiblePyMembers(sipSpec *pt, classDef *cd);
static void getVirtuals(sipSpec *pt, classDef *cd);
static void addVirtual(sipSpec *pt, overDef *od, classDef *cd);
static virtErrorHandler *getVirtErrorHandler(sipSpec *pt, overDef *od,
        classDef *cd);
static virtHandlerDef *getVirtualHandler(sipSpec *pt, overDef *od,
        classDef *cd);
static int checkVirtualHandler(overDef *od, virtHandlerDef *vhd);
static void transformTypedefs(sipSpec *pt, moduleDef *mod);
static void resolveMappedTypeTypes(sipSpec *,mappedTypeDef *);
static void resolveCtorTypes(sipSpec *,classDef *,ctorDef *);
static void resolveFuncTypes(sipSpec *pt, moduleDef *mod, classDef *c_scope,
        overDef *od);
static void resolvePySigTypes(sipSpec *,moduleDef *,classDef *,overDef *,signatureDef *,int);
static void resolveVariableType(sipSpec *,varDef *);
SIP_NORETURN static void fatalNoDefinedType(scopedNameDef *);
static void resolveType(sipSpec *,moduleDef *,classDef *,argDef *,int);
static void setNeededType(argDef *ad);
static void setNeededExceptions(sipSpec *pt, moduleDef *mod,
        throwArgs *exceptions);
static void setNeedsException(exceptionDef *xd);
static void searchClassScope(sipSpec *pt, classDef *c_scope,
        scopedNameDef *snd, argDef *ad);
static void searchScope(sipSpec *pt, classDef *scope, scopedNameDef *snd,
        argDef *ad);
static void nameLookup(sipSpec *pt, moduleDef *context, scopedNameDef *snd,
        argDef *ad);
static void searchMappedTypes(sipSpec *,moduleDef *,scopedNameDef *,argDef *);
static void searchEnums(sipSpec *,scopedNameDef *,argDef *);
static void searchClasses(sipSpec *,moduleDef *mod,scopedNameDef *,argDef *);
static mroDef *newMRO(classDef *cd);
static void moveMainModuleCastsSlots(sipSpec *pt, moduleDef *mod);
static void moveClassCasts(sipSpec *pt, moduleDef *mod, classDef *cd);
static void moveGlobalSlot(sipSpec *pt, moduleDef *mod, memberDef *gmd);
static classDef *findAltClassImplementation(sipSpec *pt, mappedTypeDef *mtd);
static ifaceFileDef *getIfaceFile(argDef *ad);
static ifaceFileDef *getIfaceFileForEnum(enumDef *ed);
static void instantiateMappedTypeTemplate(sipSpec *pt, moduleDef *mod,
        mappedTypeTmplDef *mtt, argDef *type);
static classDef *getProxy(moduleDef *mod, classDef *cd);
static int generatingCodeForModule(sipSpec *pt, moduleDef *mod);
static void checkAssignmentHelper(sipSpec *pt, classDef *cd);
static void addComplementarySlots(sipSpec *pt, classDef *cd);
static void addComplementarySlot(sipSpec *pt, classDef *cd, memberDef *md,
        slotType cslot, const char *cslot_name);
static void resolveInstantiatedClassTemplate(sipSpec *pt, argDef *type);
static void setStringPoolOffsets(sipSpec *pt);
static mappedTypeDef *copyTemplateType(mappedTypeDef *mtd, argDef *ad);
static void checkProperties(classDef *cd);


/*
 * Transform the parse tree.
 */

void transform(sipSpec *pt, int strict)
{
    moduleDef *mod;
    classDef *cd, *rev, **tail;
    classList *newl;
    overDef *od;
    exceptionDef *xd;

    /*
     * The class list has the main module's classes at the front and the ones
     * from the module at the most nested %Import at the end.  This affects
     * some of the following algorithms.  We have to have consistency whenever
     * a module is used.  To achieve this we reverse the order of the classes.
     */
    rev = NULL;
    cd = pt -> classes;

    while (cd != NULL)
    {
        classDef *next;

        /*
         * Take the opportunity to strip any classes that are only template
         * arguments.
         */
        while (isTemplateArg(cd))
            if ((cd = cd->next) == NULL)
                break;

        if (cd == NULL)
            break;

        next = cd -> next;

        cd -> next = rev;
        rev = cd;

        /*
         * Mark any QObject class.  This flag will ripple through all derived
         * classes when we set the hierarchy.
         */
        if (strcmp(classBaseName(cd), "QObject") == 0)
            setIsQObjectSubClass(cd);

        cd = next;
    }

    pt -> classes = rev;

    /*
     * Build the list of all imports for each module and check each has been
     * named.
     */
    for (mod = pt->modules; mod != NULL; mod = mod->next)
    {
        if (mod->name == NULL)
            fatal("A module is missing a %%Module or %%CModule directive\n");

        setAllImports(mod);
    }

    /*
     * Set the default meta-type for the main module if it doesn't have one
     * explicitly set.
     */
    if (pt->module->defmetatype == NULL)
    {
        moduleListDef *mld;

        for (mld = pt->module->allimports; mld != NULL; mld = mld->next)
        {
            if (mld->module->defmetatype == NULL)
                continue;

            if (pt->module->defmetatype == NULL)
                pt->module->defmetatype = mld->module->defmetatype;
            else if (pt->module->defmetatype != mld->module->defmetatype)
                fatal("The %s module has imported different default meta-types %s and %s\n",
                        pt->module->fullname->text,
                        pt->module->defmetatype->text,
                        mld->module->defmetatype->text);
        }
    }

    /* Check each class has been defined. */
    for (cd = pt -> classes; cd != NULL; cd = cd -> next)
        if (cd -> iff -> module == NULL)
        {
            fatalScopedName(classFQCName(cd));
            fatal(" has not been defined\n");
        }

    /*
     * Set the super-class hierarchy for each class and re-order the list of
     * classes so that no class appears before a super class or an enclosing
     * scope class.
     */
    newl = NULL;

    for (cd = pt -> classes; cd != NULL; cd = cd -> next)
        setHierarchy(pt,cd,cd,&newl);

    /* Replace the old list with the new one. */
    tail = &pt -> classes;

    while (newl != NULL)
    {
        classList *cl = newl;

        *tail = cl -> cd;
        tail = &cl -> cd -> next;

        newl = cl -> next;
        free(cl);
    }

    *tail = NULL;

    /* Transform the various types in the modules. */
    if (isConsolidated(pt->module))
    {
        /* Transform the modules included by the consolidated module. */
        for (mod = pt->modules->next; mod != NULL; mod = mod->next)
            transformModules(pt, strict, mod);
    }
    else
    {
        transformModules(pt, strict, pt->modules);
    }

    /* Handle default ctors now that the argument types are resolved. */ 
    if (!pt->genc)
        for (cd = pt->classes; cd != NULL; cd = cd->next)
            if (!noDefaultCtors(cd) && !isOpaque(cd) && cd->iff->type != namespace_iface)
                addDefaultCopyCtor(cd);

    /* Add any automatically generated methods. */
    for (cd = pt -> classes; cd != NULL; cd = cd -> next)
        for (od = cd -> overs; od != NULL; od = od -> next)
            if (isAutoGen(od))
                addAutoOverload(pt,cd,od);

    /*
     * Move casts and slots around to their correct classes (if in the same
     * module) or create proxies for them (if cross-module).
     */
    if (!pt->genc)
        for (mod = pt->modules; mod != NULL; mod = mod->next)
            if (generatingCodeForModule(pt, mod))
                moveMainModuleCastsSlots(pt, mod);

    /* Automatically generate missing complementary slots. */
    if (!pt->genc)
    {
        for (cd = pt->classes; cd != NULL; cd = cd->next)
            addComplementarySlots(pt, cd);

        for (mod = pt->modules; mod != NULL; mod = mod->next)
            if (generatingCodeForModule(pt, mod))
                for (cd = mod->proxies; cd != NULL; cd = cd->next)
                    addComplementarySlots(pt, cd);
    }

    /* Generate the different class views. */
    for (cd = pt->classes; cd != NULL; cd = cd->next)
        if (cd->iff->type == class_iface)
        {
            if (needsShadow(cd) && !isIncomplete(cd) && !isPrivateDtor(cd) && canCreate(cd))
                setHasShadow(cd);

            /* Get the list of visible Python member functions. */
            getVisiblePyMembers(pt, cd);

            /* Get the virtual members. */
            if (needsShadow(cd))
                getVirtuals(pt, cd);
        }
        else if (cd->iff->type == namespace_iface)
        {
            for (od = cd->overs; od != NULL; od = od->next)
                ifaceFilesAreUsedByOverload(&cd->iff->used, od, FALSE);
        }

    for (mod = pt->modules; mod != NULL; mod = mod->next)
    {
        /* Create the array of numbered types sorted by type name. */
        createSortedNumberedTypesTable(pt, mod);

        for (od = mod->overs; od != NULL; od = od->next)
            ifaceFilesAreUsedByOverload(&mod->used, od, FALSE);

        /* Update proxies with some information from the real classes. */
        for (cd = mod->proxies; cd != NULL; cd = cd->next)
            cd->iff->ifacenr = cd->real->iff->ifacenr;
    }

    /* Additional class specific checks. */
    for (cd = pt->classes; cd != NULL; cd = cd->next)
    {
        checkAssignmentHelper(pt, cd);
        checkProperties(cd);
    }

    /* Number the exceptions as they will be seen by the main module. */
    for (xd = pt->exceptions; xd != NULL; xd = xd->next)
    {
        moduleDef *xd_mod;

        /*
         * Skip those that don't require a Python exception object to be
         * created.
         */
        if (xd->iff->type != exception_iface)
            continue;

        if (xd->bibase == NULL && xd->base == NULL)
            continue;

        xd_mod = xd->iff->module;

        if (xd_mod == pt->module || xd->needed)
            xd->exceptionnr = xd_mod->nrexceptions++;
    }

    setStringPoolOffsets(pt);
}


/*
 * Transform a module and the modules it imports.
 */
static void transformModules(sipSpec *pt, int strict, moduleDef *mod)
{
    classDef *cd;
    moduleListDef *mld;

    /* Handle the trivial case. */
    if (isTransformed(mod))
        return;

    /*
     * The modules on which this one depends must be done first because they
     * might generate new template-based types and they must be defined in the
     * right module.
     */
    for (mld = mod->imports; mld != NULL; mld = mld->next)
        transformModules(pt, strict, mld->module);

    /* Transform typedefs, variables and global functions. */
    transformTypedefs(pt, mod);
    transformVariableList(pt, mod);
    transformScopeOverloads(pt, strict, NULL, NULL, mod->overs);

    /* Transform class ctors, functions and casts. */
    for (cd = pt->classes; cd != NULL; cd = cd->next)
    {
        if (cd->iff->module == mod)
        {
            transformCtors(pt, cd);

            /* Handle any dtor exceptions. */
            setNeededExceptions(pt, mod, cd->dtorexceptions);

            if (!pt->genc)
            {
                transformScopeOverloads(pt, strict, cd, NULL, cd->overs);
                transformCasts(pt, cd);
            }
        }
    }

    /* Transform mapped types based on templates. */
    transformMappedTypes(pt, strict, mod);

    setIsTransformed(mod);
}


/*
 * Set the offset into the string pool for every used name.
 */
static void setStringPoolOffsets(sipSpec *pt)
{
    nameDef *nd;
    size_t offset = 0;

    for (nd = pt->namecache; nd != NULL; nd = nd->next)
    {
        size_t len;
        nameDef *prev;

        if (!isUsedName(nd))
            continue;

        /* See if the tail of a previous used name could be used instead. */
        len = nd->len;

        for (prev = pt->namecache; prev->len > len; prev = prev->next)
        {
            size_t pos;

            if (!isUsedName(prev) || isSubstring(prev))
                continue;

            pos = prev->len - len;

            if (memcmp(&prev->text[pos], nd->text, len) == 0)
            {
                setIsSubstring(nd);
                nd->offset = prev->offset + pos;
                break;
            }
        }

        if (!isSubstring(nd))
        {
            nd->offset = offset;
            offset += len + 1;
        }
    }
}


/*
 * Add any missing complementary slots to a class.  This emulates the C++
 * behaviour of automatically interpreting (for example) >= as !<.
 */
static void addComplementarySlots(sipSpec *pt, classDef *cd)
{
    memberDef *md;

    for (md = cd->members; md != NULL; md = md->next)
        switch (md->slot)
        {
        case lt_slot:
            addComplementarySlot(pt, cd, md, ge_slot, "__ge__");
            break;

        case le_slot:
            addComplementarySlot(pt, cd, md, gt_slot, "__gt__");
            break;

        case gt_slot:
            addComplementarySlot(pt, cd, md, le_slot, "__le__");
            break;

        case ge_slot:
            addComplementarySlot(pt, cd, md, lt_slot, "__lt__");
            break;

        case eq_slot:
            addComplementarySlot(pt, cd, md, ne_slot, "__ne__");
            break;

        case ne_slot:
            addComplementarySlot(pt, cd, md, eq_slot, "__eq__");
            break;

        /* Suppress a compiler warning. */
        default:
            ;
        }
}


/*
 * Add a complementary slot if it is missing.
 */
static void addComplementarySlot(sipSpec *pt, classDef *cd, memberDef *md,
        slotType cslot, const char *cslot_name)
{
    overDef *od1;
    memberDef *md2 = NULL;

    for (od1 = cd->overs; od1 != NULL; od1 = od1->next)
    {
        overDef *od2;

        if (od1->common != md || isComplementary(od1) || od1->methodcode != NULL)
            continue;

        /* Try and find an existing complementary slot. */
        for (od2 = cd->overs; od2 != NULL; od2 = od2->next)
            if (od2->common->slot == cslot && sameSignature(&od1->pysig, &od2->pysig, TRUE))
                break;

        /*
         * If there is an explicit complementary slot then there is nothing to
         * do.
         */
        if (od2 != NULL)
            continue;

        /* Create a new member if needed. */
        if (md2 == NULL)
        {
            for (md2 = cd->members; md2 != NULL; md2 = md2->next)
                if (md2->slot == cslot)
                    break;

            if (md2 == NULL)
            {
                md2 = sipMalloc(sizeof (memberDef));

                md2->pyname = cacheName(pt, cslot_name);
                md2->memberflags = md->memberflags;
                md2->slot = cslot;
                md2->module = md->module;

                md2->next = cd->members;
                cd->members = md2;

                if (isUsedName(md->pyname))
                    setIsUsedName(md2->pyname);
            }
        }

        /* Create the complementary slot. */
        od2 = sipMalloc(sizeof (overDef));

        *od2 = *od1;
        resetIsVirtual(od2);
        setIsComplementary(od2);
        od2->common = md2;
        od2->cppname = cslot_name;

        od2->next = cd->overs;
        cd->overs = od2;
    }
}


/*
 * See if a class supports an assignment helper.
 */
static void checkAssignmentHelper(sipSpec *pt, classDef *cd)
{
    int pub_def_ctor, pub_copy_ctor;
    ctorDef *ct;

    /*
     * We register types with Qt if the class is not abstract, doesn't have a
     * private assignment operator, has a public default ctor, a public copy
     * ctor and a public dtor.
     */
    if (isAbstractClass(cd))
        return;

    if (cannotAssign(cd))
        return;

    if (!isPublicDtor(cd))
        return;

    pub_def_ctor = pub_copy_ctor = FALSE;

    for (ct = cd->ctors; ct != NULL; ct = ct->next)
    {
        if (ct->cppsig == NULL || !isPublicCtor(ct))
            continue;

        if (ct->cppsig->nrArgs == 0 || ct->cppsig->args[0].defval != NULL)
        {
            /*
             * The ctor either has no arguments or all arguments have defaults.
             */
            pub_def_ctor = TRUE;
        }
        else if (ct->cppsig->nrArgs == 1)
        {
            argDef *ad = &ct->cppsig->args[0];
            classDef *arg_cd;

            if (ad->atype == class_type)
                arg_cd = ad->u.cd;
            else if (ad->atype == mapped_type)
                arg_cd = findAltClassImplementation(pt, ad->u.mtd);
            else
                arg_cd = NULL;

            if (arg_cd == cd && isReference(ad) && isConstArg(ad) &&
                ad->nrderefs == 0 && ad->defval == NULL)
                pub_copy_ctor = TRUE;
        }
    }

    if (pub_def_ctor && pub_copy_ctor)
    {
        setAssignmentHelper(cd);
        appendToIfaceFileList(&cd->iff->module->used, cd->iff);
    }
}


/*
 * Set the list of all imports for a module.  The list is ordered so that a
 * module appears before any module that imports it.
 */
static void setAllImports(moduleDef *mod)
{
    moduleListDef *mld;

    /*
     * Handle the trivial case where there are no imports, or the list has
     * already been done.
     */
    if (mod->imports == NULL || mod->allimports != NULL)
        return;

    /* Check for recursive imports. */
    if (settingImports(mod))
    {
        fatalStart();
        fatal("Module %s is imported recursively\n", mod->name);
    }

    setSettingImports(mod);

    /* Make sure all the direct imports are done first. */
    for (mld = mod->imports; mld != NULL; mld = mld->next)
        setAllImports(mld->module);

    /*
     * Now build the list from our direct imports lists but ignoring
     * duplicates.
     */
    for (mld = mod->imports; mld != NULL; mld = mld->next)
    {
        moduleListDef *amld;

        for (amld = mld->module->allimports; amld != NULL; amld = amld->next)
            addUniqueModule(mod, amld->module);

        addUniqueModule(mod, mld->module);
    }

    resetSettingImports(mod);
}


/*
 * Append a module to the list of all imported modules if it isn't already
 * there.
 */
static void addUniqueModule(moduleDef *mod, moduleDef *imp)
{
    moduleListDef **tail;

    for (tail = &mod->allimports; *tail != NULL; tail = &(*tail)->next)
        if ((*tail)->module == imp)
            return;

    *tail = sipMalloc(sizeof (moduleListDef));

    (*tail)->module = imp;
    (*tail)->next = NULL;
}


/*
 * Move the casts and slots to the correct place for a main module (ie. one we
 * are generating code for).
 */
static void moveMainModuleCastsSlots(sipSpec *pt, moduleDef *mod)
{
    classDef *cd;
    memberDef *md;

    for (cd = pt->classes; cd != NULL; cd = cd->next)
        if (cd->iff->module == mod)
            moveClassCasts(pt, mod, cd);

    for (md = mod->othfuncs; md != NULL; md = md->next)
        if (md->slot != no_slot && md->module == mod)
            moveGlobalSlot(pt, mod, md);
}


/*
 * Move any class casts to its correct class, or publish as a ctor extender.
 */
static void moveClassCasts(sipSpec *pt, moduleDef *mod, classDef *cd)
{
    argList *al;

    for (al = cd->casts; al != NULL; al = al->next)
    {
        classDef *dcd = al->arg.u.cd;
        ctorDef *ct, **ctp;
        argDef *ad;

        if (al->arg.atype == class_type)
            dcd = al->arg.u.cd;
        else
            /* Previous error checking means this will always work. */
            dcd = findAltClassImplementation(pt, al->arg.u.mtd);

        /* Create the new ctor. */
        ct = sipMalloc(sizeof (ctorDef));

        ct->ctorflags = SECT_IS_PUBLIC | CTOR_CAST;
        ct->cppsig = &ct->pysig;

        /* Add the source class as the only argument. */
        ct->pysig.result.atype = void_type;
        ad = &ct->pysig.args[0];

        ad->atype = class_type;
        ad->name = NULL;
        ad->argflags = ARG_IN | (al->arg.argflags & (ARG_IS_REF | ARG_IS_CONST));
        ad->nrderefs = al->arg.nrderefs;
        memcpy(ad->derefs, al->arg.derefs, sizeof (ad->derefs));
        ad->defval = NULL;
        ad->u.cd = cd;

        /*
         * If the destination class is in a different module then use
         * a proxy.
         */
        if (dcd->iff->module != mod)
        {
            ifaceFileIsUsed(&mod->used, ad, FALSE);
            dcd = getProxy(mod, dcd);
            ct->no_typehint = TRUE;
        }

        ifaceFileIsUsed(&dcd->iff->used, ad, FALSE);

        ct->pysig.nrArgs = 1;

        /* Append it to the list. */
        for (ctp = &dcd->ctors; *ctp != NULL; ctp = &(*ctp)->next)
            if (sameSignature(&(*ctp)->pysig, &ct->pysig, FALSE))
            {
                fatalStart();
                fprintf(stderr, "operator ");
                fatalScopedName(classFQCName(dcd));
                fprintf(stderr, "::");
                fatalScopedName(classFQCName(dcd));
                fprintf(stderr, "(");
                fatalScopedName(classFQCName(cd));
                fatal(") already defined\n");
            }

        *ctp = ct;
    }
}


/*
 * If possible, move a global slot to its correct class.
 */
static void moveGlobalSlot(sipSpec *pt, moduleDef *mod, memberDef *gmd)
{
    overDef **odp = &mod->overs, *od;

    while ((od = *odp) != NULL)
    {
        int second;
        argDef *arg0, *arg1;
        memberDef *md, **mdhead;
        overDef **odhead;
        moduleDef *mod;
        enumDef *ed;

        if (od->common != gmd)
        {
            odp = &od->next;
            continue;
        }

        /*
         * We know that the slot has the right number of arguments, but the
         * first or second one needs to be a class or enum defined in the same
         * module.  Otherwise we leave it as it is and publish it as a slot
         * extender.
         */
        arg0 = &od->pysig.args[0];
        arg1 = &od->pysig.args[1];

        mdhead = NULL;
        second = FALSE;
        ed = NULL;

        if (arg0->atype == class_type)
        {
            mdhead = &arg0->u.cd->members;
            odhead = &arg0->u.cd->overs;
            mod = arg0->u.cd->iff->module;
        }
        else if (arg0->atype == mapped_type)
        {
            classDef *cd = findAltClassImplementation(pt, arg0->u.mtd);

            if (cd != NULL)
            {
                mdhead = &cd->members;
                odhead = &cd->overs;
                mod = cd->iff->module;
            }
        }
        else if (arg0->atype == enum_type)
        {
            mdhead = &arg0->u.ed->slots;
            odhead = &arg0->u.ed->overs;
            mod = arg0->u.ed->module;
            ed = arg0->u.ed;
        }
        else if (arg1->atype == class_type)
        {
            mdhead = &arg1->u.cd->members;
            odhead = &arg1->u.cd->overs;
            mod = arg1->u.cd->iff->module;
            second = TRUE;
        }
        else if (arg1->atype == mapped_type)
        {
            classDef *cd = findAltClassImplementation(pt, arg1->u.mtd);

            if (cd != NULL)
            {
                mdhead = &cd->members;
                odhead = &cd->overs;
                mod = cd->iff->module;
                second = TRUE;
            }
        }
        else if (arg1->atype == enum_type)
        {
            mdhead = &arg1->u.ed->slots;
            odhead = &arg1->u.ed->overs;
            mod = arg1->u.ed->module;
            ed = arg1->u.ed;
            second = TRUE;
        }

        if (mdhead == NULL)
        {
            fatalStart();
            fprintf(stderr, "%s:%d: One of the arguments of ", od->sloc.name,
                    od->sloc.linenr);
            prOverloadName(stderr, od);
            fatal(" must be a class or enum\n");
        }

        /*
         * For rich comparisons the first argument must be a class or an enum.
         * For cross-module slots then it may only be a class.  (This latter
         * limitation is artificial, but is unlikely to be a problem in
         * practice.)
         */
        if (isRichCompareSlot(gmd))
        {
            if (second)
            {
                fatalStart();
                fprintf(stderr, "%s:%d: Argument 1 of ", od->sloc.name,
                        od->sloc.linenr);
                prOverloadName(stderr, od);
                fatal(" must be a class or enum\n");
            }

            if (mod != gmd->module && arg0->atype == enum_type)
            {
                fatalStart();
                fprintf(stderr, "%s:%d: Argument 1 of ", od->sloc.name,
                        od->sloc.linenr);
                prOverloadName(stderr, od);
                fatal(" must be a class\n");
            }
        }

        if (mod != gmd->module)
        {
            if (isRichCompareSlot(gmd))
            {
                classDef *pcd = getProxy(mod, arg0->u.cd);
                memberDef *pmd;
                overDef *pod;

                /* Create a new proxy member if needed. */
                for (pmd = pcd->members; pmd != NULL; pmd = pmd->next)
                    if (pmd->slot == gmd->slot)
                        break;

                if (pmd == NULL)
                {
                    pmd = sipMalloc(sizeof (memberDef));

                    pmd->pyname = gmd->pyname;
                    pmd->memberflags = 0;
                    pmd->slot = gmd->slot;
                    pmd->module = mod;
                    pmd->ns_scope = gmd->ns_scope;
                    pmd->next = pcd->members;

                    pcd->members = pmd;
                }

                /* Add the proxy overload. */
                pod = sipMalloc(sizeof (overDef));

                *pod = *od;
                pod->no_typehint = TRUE;
                pod->common = pmd;
                pod->next = pcd->overs;

                pcd->overs = pod;

                /* Remove the first argument. */
                pod->pysig.args[0] = pod->pysig.args[1];
                pod->pysig.nrArgs = 1;

                /* Remove from the list. */
                *odp = od->next;
            }
            else
                odp = &od->next;

            continue;
        }

        /* Remove from the list. */
        *odp = od->next;

        if (ed != NULL)
        {
            ifaceFileDef *enum_iff = getIfaceFileForEnum(ed);

            /* The slot code is generated at the module level. */
            if (enum_iff != NULL)
                appendToIfaceFileList(&mod->used, enum_iff);

            setIsUsedName(ed->pyname);
        }

        /* See if there is already a member or create a new one. */
        for (md = *mdhead; md != NULL; md = md->next)
            if (md->slot == gmd->slot)
                break;

        if (md == NULL)
        {
            md = sipMalloc(sizeof (memberDef));

            *md = *gmd;

            md->module = mod;
            md->next = *mdhead;

            *mdhead = md;
        }

        /* Move the overload to the end of the destination list. */
        if (second)
            setIsReflected(od);

        setIsPublic(od);
        setIsGlobal(od);
        od->common = md;
        od->next = NULL;

        while (*odhead != NULL)
            odhead = &(*odhead)->next;

        *odhead = od;

        /*
         * Remove the first argument of inplace numeric operators and
         * comparison operators.
         */
        if (isInplaceNumberSlot(md) || isRichCompareSlot(md))
        {
            /* Remember if the argument was a pointer. */
            if (arg0->nrderefs > 0)
                setDontDerefSelf(od);

            *arg0 = *arg1;
            od->pysig.nrArgs = 1;
        }

        /* Remove the only argument of unary operators. */
        if (isZeroArgSlot(md))
            od->pysig.nrArgs = 0;
    }
}


/*
 * Return an alternative class implementation of a mapped type if there is
 * one.  Note that we cheat as we assume there is one going to be one (as
 * there will be in PyQt at the moment).
 */
static classDef *findAltClassImplementation(sipSpec *pt, mappedTypeDef *mtd)
{
    ifaceFileDef *iff = mtd->iff->first_alt;

    while (iff != NULL)
    {
        if (iff->type == class_iface)
        {
            classDef *cd;

            for (cd = pt->classes; cd != NULL; cd = cd->next)
                if (cd->iff == iff)
                    return cd;
        }

        iff = iff->next_alt;
    }

    return NULL;
}


/*
 * Create a proxy for a class if it doesn't already exist.  Proxies are used as
 * containers for cross-module extenders.
 */
static classDef *getProxy(moduleDef *mod, classDef *cd)
{
    classDef *pcd;

    for (pcd = mod->proxies; pcd != NULL; pcd = pcd->next)
        if (pcd->iff == cd->iff)
            return pcd;

    pcd = sipMalloc(sizeof (classDef));

    pcd->pyname = cd->pyname;
    pcd->iff = cd->iff;
    pcd->ecd = cd->ecd;
    pcd->real = cd;
    pcd->supers = cd->supers;
    pcd->mro = cd->mro;
    pcd->next = mod->proxies;

    mod->proxies = pcd;

    return pcd;
}


/*
 * Add an overload that is automatically generated (typically by Qt's moc).
 */
static void addAutoOverload(sipSpec *pt,classDef *autocd,overDef *autood)
{
    classDef *cd;

    /* Find every class that has this one in its hierarchy. */

    for (cd = pt -> classes; cd != NULL; cd = cd -> next)
    {
        mroDef *mro;

        if (cd == autocd)
            continue;

        for (mro = cd -> mro; mro != NULL; mro = mro -> next)
            if (mro -> cd == autocd)
            {
                memberDef *md;
                overDef *od;

                /* Another overload may already exist. */

                for (md = cd -> members; md != NULL; md = md -> next)
                    if (md -> pyname == autood -> common -> pyname)
                        break;

                if (md == NULL)
                {
                    md = sipMalloc(sizeof (memberDef));

                    md -> pyname = autood -> common -> pyname;
                    md -> memberflags = autood -> common -> memberflags;
                    md -> slot = autood -> common -> slot;
                    md -> module = cd -> iff -> module;
                    md -> next = cd -> members;
                    cd -> members = md;
                }

                od = sipMalloc(sizeof (overDef));

                *od = *autood;
                od -> common = md;
                od -> next = cd -> overs;
                cd -> overs = od;

                resetIsAutoGen(od);

                if (generatingCodeForModule(pt, cd->iff->module))
                    setIsUsedName(md -> pyname);

                break;
            }
    }
}


/*
 * Set the complete hierarchy for a class.
 */
static void setHierarchy(sipSpec *pt, classDef *base, classDef *cd,
        classList **head)
{
    /* See if it has already been done. */
    if (cd->mro != NULL)
        return;

    if (cd->ecd != NULL)
    {
        setHierarchy(pt, base, cd->ecd, head);

        if (isDeprecatedClass(cd->ecd))
            setIsDeprecatedClass(cd);
    }

    if (cd->iff->type == class_iface)
    {
        classList *cl;

        /* The first thing is itself. */
        cd->mro = newMRO(cd);

        if (cd->convtosubcode != NULL)
            cd->subbase = cd;

        /* Now do it's superclasses. */
        setHierBeingSet(cd->mro);

        for (cl = cd->supers; cl != NULL; cl = cl->next)
        {
            mroDef *mro;

            if (cl->cd->mro != NULL && hierBeingSet(cl->cd->mro))
            {
                fatalStart();
                fprintf(stderr, "Recursive class hierarchy detected: ");
                fatalScopedName(classFQCName(cd));
                fprintf(stderr, " and ");
                fatalScopedName(classFQCName(cl->cd));
                fatal("\n");
            }

            /* Make sure the super-class's hierarchy has been done. */
            setHierarchy(pt, base, cl->cd, head);

            /* Append the super-classes hierarchy. */
            for (mro = cl->cd->mro; mro != NULL; mro = mro->next)
            {
                mroDef **tailp;

                /* See if the class is already in the hierarchy. */
                for (tailp = &cd->mro->next; *tailp != NULL; tailp = &(*tailp)->next)
                {
                    if ((*tailp)->cd == mro->cd)
                    {
                        setInADiamond(*tailp);
                        break;
                    }
                }

                if (*tailp == NULL)
                {
                    *tailp = newMRO(mro->cd);

                    if (inADiamond(mro))
                        setInADiamond(*tailp);
                }

                if (generatingCodeForModule(pt, cd->iff->module))
                    mro->cd->iff->first_alt->needed = TRUE;

                if (isDeprecatedClass(mro->cd))
                    setIsDeprecatedClass(cd);

                /*
                 * If the super-class is a QObject sub-class then this one is
                 * as well.
                 */
                if (isQObjectSubClass(mro->cd))
                    setIsQObjectSubClass(cd);

                /*
                 * If the super-class can't be assigned to then this one
                 * cannot either.
                 */
                if (cannotAssign(mro->cd))
                    setCannotAssign(cd);

                /*
                 * If the super-class needs a shadow then this one should have
                 * one as well.
                 */
                if (needsShadow(mro->cd))
                    setNeedsShadow(cd);

                /*
                 * Ensure that the sub-class base class is the furthest up the
                 * hierarchy.
                 */
                if (mro->cd->subbase != NULL)
                    cd->subbase = mro->cd->subbase;
            }
        }

        resetHierBeingSet(cd->mro);

        /*
         * If the class doesn't have an explicit meta-type then inherit from
         * the module's default.
         */
        if (cd->metatype == NULL && cd->supers == NULL)
            cd->metatype = cd->iff->module->defmetatype;

        if (cd->metatype != NULL && generatingCodeForModule(pt, cd->iff->module))
            setIsUsedName(cd->metatype);

        /*
         * If the class doesn't have an explicit super-type then inherit from
         * the module's default.
         */
        if (cd->supertype == NULL && cd->supers == NULL)
            cd->supertype = cd->iff->module->defsupertype;

        if (cd->supertype != NULL && strcmp(cd->supertype->text, "sip.wrapper") == 0)
            cd->supertype = NULL;

        if (cd->supertype != NULL && generatingCodeForModule(pt, cd->iff->module))
            setIsUsedName(cd->supertype);
    }

    /*
     * Make sure that the module in which a sub-class convertor will be created
     * knows about the base class.
     */
    if (cd->subbase != NULL)
        appendToIfaceFileList(&cd->iff->module->used, cd->subbase->iff);

    /*
     * We can't have a shadow if the specification is incomplete, there is a
     * private dtor, there are no non-private ctors or there are private
     * abstract methods.
     */
    if (isIncomplete(cd) || isPrivateDtor(cd) || !canCreate(cd))
    {
        resetHasShadow(cd);
    }
    else
    {
        overDef *od;

        /*
         * Note that we should be able to provide better support for abstract
         * private methods than we do at the moment.
         */
        for (od = cd->overs; od != NULL; od = od->next)
            if (isAbstract(od) && isPrivate(od))
            {
                resetHasShadow(cd);

                /* It also means we cannot create an instance from Python. */
                resetCanCreate(cd);

                break;
            }
    }

    /* Add it to the new list. */
    appendToClassList(head,cd);
}


/*
 * Create an MRO entry for a class.
 */
static mroDef *newMRO(classDef *cd)
{
    mroDef *mro;

    mro = sipMalloc(sizeof (mroDef));

    mro -> cd = cd;
    mro -> mroflags = 0;
    mro -> next = NULL;

    return mro;
}


/*
 * Get the base types for all typedefs of a module.
 */
static void transformTypedefs(sipSpec *pt, moduleDef *mod)
{
    typedefDef *td;

    for (td = pt->typedefs; td != NULL; td = td->next)
        if (td->module == mod)
            if (td->ecd == NULL || !isTemplateClass(td->ecd))
                resolveType(pt, td->module, td->ecd, &td->type, FALSE);
}


/*
 * Transform the data types for mapped types based on a template.
 */
static void transformMappedTypes(sipSpec *pt, int strict, moduleDef *mod)
{
    mappedTypeDef *mt;

    for (mt = pt->mappedtypes; mt != NULL; mt = mt->next)
    {
        if (mt->iff->module == mod)
        {
            if (mt->type.atype == template_type)
                resolveMappedTypeTypes(pt, mt);
            else
                transformScopeOverloads(pt, strict, NULL, mt, mt->overs);
        }
    }
}


/*
 * Transform the data types for a list of ctors.
 */
static void transformCtors(sipSpec *pt, classDef *cd)
{
    ctorDef *ct;

    for (ct = cd->ctors; ct != NULL; ct = ct->next)
    {
        ctorDef *prev;

        resolveCtorTypes(pt, cd, ct);

        /*
         * Now check that the Python signature doesn't conflict with an
         * earlier one.  If there is %MethodCode then assume that it will
         * handle any potential conflicts.
         */
        if (ct->methodcode == NULL)
        {
            for (prev = cd->ctors; prev != ct; prev = prev->next)
            {
                if (prev->methodcode != NULL)
                    continue;

                if (samePythonSignature(&prev->pysig, &ct->pysig))
                {
                    fatalScopedName(classFQCName(cd));
                    fatal(" has ctors with the same Python signature\n");
                }
            }
        }

        if (isDeprecatedClass(cd))
            setIsDeprecatedCtor(ct);
    }
}


/*
 * Transform the data type for a list of casts.
 */
static void transformCasts(sipSpec *pt, classDef *cd)
{
    argList *al;

    for (al = cd->casts; al != NULL; al = al->next)
    {
        classDef *dcd;

        resolveType(pt, cd->iff->module, cd, &al->arg, FALSE);

        if (al->arg.atype == class_type)
            dcd = al->arg.u.cd;
        else if (al->arg.atype == mapped_type)
            dcd = findAltClassImplementation(pt, al->arg.u.mtd);
        else
            dcd = NULL;

        if (dcd == NULL)
        {
            fatalScopedName(classFQCName(cd));
            fatal(" operator cast must be to a class\n");
        }
    }
}


/*
 * Add a default copy ctor if required.
 */
static void addDefaultCopyCtor(classDef *cd)
{
    ctorDef *copyct, **tailp;
    mroDef *mro;

    /* See if there is a private copy ctor in the hierarchy. */
    for (mro = cd->mro; mro != NULL; mro = mro->next)
    {
        ctorDef *ct;

        for (ct = mro->cd->ctors; ct != NULL; ct = ct->next)
        {
            argDef *ad = &ct -> pysig.args[0];
 
            /* See if is a copy ctor. */
            if (ct->pysig.nrArgs == 1 && ad->nrderefs == 0 && isReference(ad))
            {
                ifaceFileDef *iff;

                /* To check the type we have to look at all versions. */
                if (ad->atype == class_type)
                    iff = ad->u.cd->iff;
                else if (ad->atype == mapped_type)
                    iff = ad->u.mtd->iff;
                else
                    continue;

                for (iff = iff->first_alt; iff != NULL; iff = iff->next_alt)
                    if (mro->cd->iff == iff)
                        break;

                if (iff != NULL)
                    break;
            }
        }

        if (ct != NULL)
        {
            /* If the copy ctor is private then the class can't be copied. */
            if (isPrivateCtor(ct))
            {
                setCannotCopy(cd);
                return;
            }

            /*
             * If the ctor is in the class itself then there is nothing to do.
             */
            if (mro == cd->mro)
                return;
 
            /* Otherwise we need to create a default. */
            break;
        }
    }
 
    /* Create a default public copy ctor. */
    copyct = sipMalloc(sizeof (ctorDef));
 
    copyct->ctorflags = SECT_IS_PUBLIC;
    copyct->pysig.nrArgs = 1;
    copyct->pysig.result.atype = void_type;
    copyct->pysig.args[0].atype = class_type;
    copyct->pysig.args[0].u.cd = cd;
    copyct->pysig.args[0].argflags = (ARG_IS_REF | ARG_IS_CONST | ARG_IN);
    copyct->pysig.args[0].nrderefs = 0;
    copyct->pysig.args[0].defval = NULL;
 
    copyct->cppsig = &copyct->pysig;
 
    if (isDeprecatedClass(cd))
        setIsDeprecatedCtor(copyct);

    if (!isAbstractClass(cd))
        setCanCreate(cd);

    /* Append it to the list. */
    for (tailp = &cd->ctors; *tailp != NULL; tailp = &(*tailp)->next)
        ;
 
    *tailp = copyct;
}


/*
 * Transform the data types for a list of overloads.
 */
static void transformScopeOverloads(sipSpec *pt, int strict, classDef *c_scope,
        mappedTypeDef *mt_scope, overDef *overs)
{
    overDef *od;

    for (od = overs; od != NULL; od = od->next)
    {
        overDef *prev;

        resolveFuncTypes(pt, od->common->module, c_scope, od);

        /*
         * Now check that the Python signature doesn't conflict with an earlier
         * one.  If there is %MethodCode then assume that it will handle any
         * potential conflicts.
         */
        if (od->methodcode == NULL && strict)
        {
            for (prev = overs; prev != od; prev = prev->next)
            {
                if (prev->common != od->common)
                    continue;

                if (prev->methodcode != NULL)
                    continue;

                /* They can only conflict if one is unversioned. */
                if (prev->api_range != NULL && od->api_range != NULL)
                    continue;

                if (samePythonSignature(&prev->pysig, &od->pysig))
                {
                    ifaceFileDef *iff;

                    fatalStart();
                    fprintf(stderr, "%s:%d: ", od->sloc.name, od->sloc.linenr);

                    if (mt_scope != NULL)
                        iff = mt_scope->iff;
                    else if (c_scope != NULL)
                        iff = c_scope->iff;
                    else
                        iff = NULL;

                    if (iff != NULL)
                    {
                        fatalScopedName(iff->fqcname);
                        fprintf(stderr, "::");
                    }

                    fatal("%s() has overloaded functions with the same Python signature\n", od->common->pyname->text);
                }
            }
        }

        if (c_scope != NULL)
        {
            if (isDeprecatedClass(c_scope))
                setIsDeprecated(od);

            if (isAbstract(od))
                setIsAbstractClass(c_scope);
        }
    }
}


/*
 * Transform the data types for the variables of a module.
 */
static void transformVariableList(sipSpec *pt, moduleDef *mod)
{
    varDef *vd;

    for (vd = pt->vars; vd != NULL; vd = vd->next)
        if (vd->module == mod)
            if (vd->ecd == NULL || !isTemplateClass(vd->ecd))
                resolveVariableType(pt, vd);
}


/*
 * Set the list of visible Python member functions for a class.
 */
static void getVisiblePyMembers(sipSpec *pt, classDef *cd)
{
    mroDef *mro;

    cd->visible = NULL;

    for (mro = cd->mro; mro != NULL; mro = mro->next)
    {
        memberDef *md;
        classDef *mro_cd;

        mro_cd = mro->cd;

        for (md = mro_cd->members; md != NULL; md = md->next)
        {
            visibleList *vl;

            /*
             * See if it is already in the list.  This has the desired side
             * effect of eliminating any functions that have an implementation
             * closer to this class in the hierarchy.  This is the only reason
             * to define private functions.
             */
            for (vl = cd->visible; vl != NULL; vl = vl->next)
                if (vl->m->pyname == md->pyname)
                    break;

            /* See if it is a new member function. */
            if (vl == NULL)
            {
                overDef *od;

                vl = sipMalloc(sizeof (visibleList));

                vl->m = md;
                vl->cd = mro_cd;
                vl->next = cd->visible;

                cd->visible = vl;

                for (od = mro_cd->overs; od != NULL; od = od->next)
                    if (od->common == md)
                    {
                        int need_types = FALSE;

                        /*
                         * If the visible overload is abstract then it hasn't
                         * had a concrete implementation so this class must
                         * also be abstract.
                         */
                        if (isAbstract(od))
                            setIsAbstractClass(cd);

                        if (generatingCodeForModule(pt, cd->iff->module) && (cd == mro_cd || (isProtected(od) && hasShadow(cd))))
                        {
                            need_types = TRUE;

                            setIsUsedName(md->pyname);

                            /* Make sure we have any API name. */
                            if (od->api_range != NULL)
                                setIsUsedName(od->api_range->api_name);
                        }

                        ifaceFilesAreUsedByOverload(&cd->iff->used, od,
                                need_types);
                    }
            }
        }
    }
}


/*
 * Get all the virtuals for a particular class.
 */
static void getVirtuals(sipSpec *pt, classDef *cd)
{
    classList *cl;
    overDef *od;

    /*
     * Copy the collected virtuals of each super-class updating from what we
     * find in this class.
     */
    for (cl = cd->supers; cl != NULL; cl = cl->next)
    {
        virtOverDef *s_vod;

        for (s_vod = cl->cd->vmembers; s_vod != NULL; s_vod = s_vod->next)
        {
            int implicit = TRUE;

            for (od = cd->overs; od != NULL; od = od->next)
            {
                if (strcmp(s_vod->od->cppname, od->cppname) != 0)
                    continue;

                implicit = FALSE;

                if (isFinal(od))
                    break;

                /* See if it re-implements rather than hides. */
                if (sameCppOverload(s_vod->od, od))
                {
                    // What if is is private?
                    setIsVirtual(od);
                    setIsVirtualReimp(od);

                    /*
                     * Use the base implementation's virtual handler code if
                     * there is any.  We cannot just use it's virtual handler
                     * because this re-implementation may have different
                     * annotations which means the complete handler would be
                     * different.  In practice there is no reason why it
                     * would be different (and maybe this should be detected as
                     * an error) but if they are the same then the same handler
                     * will eventually be chosen.
                     */
                    if (od->virtcode == NULL)
                        od->virtcode = s_vod->od->virtcode;

                    /*
                     * Use the base implementation's virtual error handler if
                     * one isn't explicitly specified.
                     */
                    if (od->virt_error_handler == NULL)
                        od->virt_error_handler = s_vod->od->virt_error_handler;

                    addVirtual(pt, od, cd);
                }
            }

            /* Add it if it wasn't explcitly mentioned in the class. */
            if (implicit)
                addVirtual(pt, s_vod->od, cd);
        }
    }

    /* Handle any new virtuals. */
    for (od = cd->overs; od != NULL; od = od->next)
        if (isVirtual(od) && !isVirtualReimp(od) && !isFinal(od))
            addVirtual(pt, od, cd);
}


/*
 * Add an overload to the list of virtuals for a class.
 */
static void addVirtual(sipSpec *pt, overDef *od, classDef *cd)
{
    virtHandlerDef *vhd;
    virtOverDef *vod;

    /*
     * If this class is defined in the main module then make sure the virtuals
     * have a handler.
     */
    if (generatingCodeForModule(pt, cd->iff->module))
    {
        vhd = getVirtualHandler(pt, od, cd);

        /* Make sure we get the name. */
        setIsUsedName(od->common->pyname);

        /*
         * Make sure we have the interface files and type definitions for the
         * virtual handler.
         */
        ifaceFilesAreUsedByOverload(&pt->module->used, od, TRUE);
    }
    else
    {
        vhd = NULL;
    }

    /* Add it to the class. */
    vod = sipMalloc(sizeof (virtOverDef));
    vod->od = od;
    vod->virthandler = vhd;
    vod->next = cd->vmembers;
 
    cd->vmembers = vod;
}

/*
 * Get the virtual error handler for a function.
 */
static virtErrorHandler *getVirtErrorHandler(sipSpec *pt, overDef *od,
        classDef *cd)
{
    const char *name;
    virtErrorHandler *veh;
    moduleDef *mod = cd->iff->module;

    /* Handle the trivial case. */
    if (noErrorHandler(od))
        return NULL;

    /* Check the function itself. */
    if ((name = od->virt_error_handler) == NULL)
    {
        mroDef *mro;

        /* Check the class hierarchy. */
        for (mro = cd->mro; mro != NULL; mro = mro->next)
            if ((name = mro->cd->virt_error_handler) != NULL)
                break;

        if (name == NULL)
        {
            /* Check the class's module. */
            if ((name = mod->virt_error_handler) == NULL)
            {
                moduleListDef *mld;

                /* Check the module hierarchy. */
                for (mld = mod->allimports; mld != NULL; mld = mld->next)
                    if ((name = mld->module->virt_error_handler) != NULL)
                        break;
            }
        }
    }

    if (name == NULL)
        return NULL;

    /* Find the handler with the name. */
    for (veh = pt->errorhandlers; veh != NULL; veh = veh->next)
        if (strcmp(veh->name, name) == 0)
            break;

    if (veh == NULL)
        fatal("Unknown virtual error handler \"%s\"\n", name);

    /* Assign it an index if we need to import the handler. */
    if (mod != veh->mod && veh->index < 0)
        veh->index = veh->mod->nrvirterrorhandlers++;

    return veh;
}


/*
 * Get the virtual handler for an overload.
 */
static virtHandlerDef *getVirtualHandler(sipSpec *pt, overDef *od, classDef *cd)
{
    virtHandlerDef *vhd;

    /* See if there is an existing handler that is suitable. */
    for (vhd = pt->virthandlers; vhd != NULL; vhd = vhd->next)
        if (checkVirtualHandler(od, vhd))
            return vhd;

    /* Create a new one. */
    vhd = sipMalloc(sizeof (virtHandlerDef));

    vhd->virthandlernr = pt->nrvirthandlers++;

    if (isFactory(od) || isResultTransferredBack(od))
        setIsTransferVH(vhd);

    if (abortOnException(od))
        setAbortOnExceptionVH(vhd);

    vhd->pysig = &od->pysig;
    vhd->cppsig = od->cppsig;
    vhd->virtcode = od->virtcode;
    vhd->veh = getVirtErrorHandler(pt, od, cd);

    vhd->next = pt->virthandlers;
    pt->virthandlers = vhd;

    return vhd;
}


/*
 * See if a virtual handler is appropriate for an overload.
 */
static int checkVirtualHandler(overDef *od, virtHandlerDef *vhd)
{
    int a;

    if (od->virtcode != vhd->virtcode)
        return FALSE;

    /*
     * If the overload has an explicit error handler then it must be the same
     * as the candidate.
     */
    if (od->virt_error_handler != NULL)
    {
        if (vhd->veh == NULL || strcmp(od->virt_error_handler, vhd->veh->name) != 0)
            return FALSE;
    }

    if ((isFactory(od) || isResultTransferredBack(od)) && !isTransferVH(vhd))
        return FALSE;

    if (!abortOnException(od) != !abortOnExceptionVH(vhd))
        return FALSE;

    if (!sameArgType(&od->pysig.result, &vhd->pysig->result, TRUE))
        return FALSE;

    if (isAllowNone(&od->pysig.result) != isAllowNone(&vhd->pysig->result))
        return FALSE;

    if (isDisallowNone(&od->pysig.result) != isDisallowNone(&vhd->pysig->result))
        return FALSE;

    if (!sameSignature(&od->pysig, vhd->pysig, TRUE))
        return FALSE;

    /* Take into account the argument directions in the Python signatures. */
    for (a = 0; a < od->pysig.nrArgs; ++a)
    {
        int dir1 = (od->pysig.args[a].argflags & (ARG_IN | ARG_OUT));
        int dir2 = (vhd->pysig->args[a].argflags & (ARG_IN | ARG_OUT));

        if (dir1 != dir2)
            return FALSE;
    }

    if (&od->pysig == od->cppsig && vhd->pysig == vhd->cppsig)
        return TRUE;

    if (!sameArgType(&od->cppsig->result, &vhd->cppsig->result, TRUE))
        return FALSE;

    return sameSignature(od->cppsig, vhd->cppsig, TRUE);
}


/*
 * Resolve the types of a mapped type based on a template.
 */
static void resolveMappedTypeTypes(sipSpec *pt, mappedTypeDef *mt)
{
    int a;
    signatureDef *sd = &mt->type.u.td->types;

    for (a = 0; a < sd->nrArgs; ++a)
    {
        argDef *ad = &sd->args[a];

        /* Leave templates as they are. */
        if (ad->atype != template_type)
            resolveType(pt, mt->iff->module, NULL, ad, TRUE);
    }

    /* Make sure that the signature result won't cause problems. */
    sd->result.atype = no_type;

    ifaceFilesAreUsedBySignature(&mt->iff->used, sd, FALSE);
}


/*
 * Resolve the types of a ctor.
 */
static void resolveCtorTypes(sipSpec *pt,classDef *scope,ctorDef *ct)
{
    int a;

    /* Handle any exceptions. */
    setNeededExceptions(pt, scope->iff->module, ct->exceptions);

    /* Handle any C++ signature. */
    if (ct->cppsig != NULL && ct->cppsig != &ct->pysig)
        for (a = 0; a < ct -> cppsig -> nrArgs; ++a)
            resolveType(pt, scope->iff->module, scope, &ct->cppsig->args[a],
                    TRUE);
 
    /* Handle the Python signature. */
    for (a = 0; a < ct -> pysig.nrArgs; ++a)
    {
        argDef *ad = &ct -> pysig.args[a];

        resolveType(pt, scope->iff->module, scope, ad, FALSE);

        if (!supportedType(scope,NULL,ad,FALSE))
        {
            fatalScopedName(classFQCName(scope));
            fatal(" ctor argument %d has an unsupported type for a Python signature - provide a valid type, %%MethodCode and a C++ signature\n", a + 1);
        }

        ifaceFileIsUsed(&scope->iff->used, ad, FALSE);
        scopeDefaultValue(pt, scope, ad);
    }
}


/*
 * Resolve the types of a function.
 */
static void resolveFuncTypes(sipSpec *pt, moduleDef *mod, classDef *c_scope,
        overDef *od)
{
    argDef *res;

    /* Handle any exceptions. */
    setNeededExceptions(pt, mod, od->exceptions);

    /* Handle any C++ signature. */
    if (od->cppsig != &od->pysig)
    {
        int a;
        argDef *res = &od->cppsig->result;

        resolveType(pt, mod, c_scope, res, TRUE);

        if ((res->atype != void_type || res->nrderefs != 0) && isVirtual(od) && !supportedType(c_scope, od, &od->cppsig->result, FALSE) && od->virtcode == NULL)
        {
            fatalStart();
            fprintf(stderr, "%s:%d: ", od->sloc.name, od->sloc.linenr);

            if (c_scope != NULL)
            {
                fatalScopedName(classFQCName(c_scope));
                fprintf(stderr, "::");
            }

            fatal("%s() unsupported virtual function return type - provide %%VirtualCatcherCode\n", od->cppname);
        }

        for (a = 0; a < od->cppsig->nrArgs; ++a)
            resolveType(pt, mod, c_scope, &od->cppsig->args[a], TRUE);
    }
 
    /* Handle the Python signature. */
    resolvePySigTypes(pt, mod, c_scope, od, &od->pysig, isSignal(od));

    res = &od->pysig.result;

    /* These slots must return SIP_SSIZE_T (or int - deprecated). */
    if (isSSizeReturnSlot(od->common))
        if ((res->atype != ssize_type && res->atype != int_type) || res->nrderefs != 0 ||
            isReference(res) || isConstArg(res))
            fatal("%s:%d: %s slots must return SIP_SSIZE_T\n", od->sloc.name,
                    od->sloc.linenr, od->common->pyname->text);

    /* These slots must return int. */
    if (isIntReturnSlot(od->common))
        if (res->atype != int_type || res->nrderefs != 0 ||
            isReference(res) || isConstArg(res))
            fatal("%s:%d: %s slots must return int\n", od->sloc.name,
                    od->sloc.linenr, od->common->pyname->text);

    /* These slots must return void. */
    if (isVoidReturnSlot(od->common))
        if (res->atype != void_type || res->nrderefs != 0 ||
            isReference(res) || isConstArg(res))
            fatal("%s:%d: %s slots must return void\n", od->sloc.name,
                    od->sloc.linenr, od->common->pyname->text);

    /* These slots must return long. */
    if (isLongReturnSlot(od->common))
        if (res->atype != long_type || res->nrderefs != 0 ||
            isReference(res) || isConstArg(res))
            fatal("%s:%d: %s slots must return long\n", od->sloc.name,
                    od->sloc.linenr, od->common->pyname->text);
}


/*
 * Resolve the types of a Python signature.
 */
static void resolvePySigTypes(sipSpec *pt, moduleDef *mod, classDef *scope,
                  overDef *od, signatureDef *pysig, int issignal)
{
    int a;
    argDef *res = &pysig -> result;

    if (res -> atype != void_type || res -> nrderefs != 0)
    {
        if (issignal)
        {
            fatalStart();
            fprintf(stderr, "%s:%d: ", od->sloc.name, od->sloc.linenr);

            if (scope != NULL)
            {
                fatalScopedName(classFQCName(scope));
                fprintf(stderr, "::");
            }

            fatal("%s() signals must return void\n", od->cppname);
        }

        resolveType(pt, mod, scope, res, FALSE);

        /* Results must be simple. */
        if (!supportedType(scope, od, res, FALSE))
        {
            int need_meth;

            need_meth = (od->cppsig == &od->pysig || od->methodcode == NULL);

            if (need_meth)
            {
                fatalStart();
                fprintf(stderr, "%s:%d: ", od->sloc.name, od->sloc.linenr);

                if (scope != NULL)
                {
                    fatalScopedName(classFQCName(scope));
                    fprintf(stderr, "::");
                }

                fatal("%s() unsupported function return type - provide %%MethodCode and a %s signature\n", od->cppname, (pt->genc ? "C" : "C++"));
            }
        }
    }

    for (a = 0; a < pysig -> nrArgs; ++a)
    {
        argDef *ad = &pysig -> args[a];

        resolveType(pt, mod, scope, ad, FALSE);

        if (ad -> atype == slotcon_type)
            resolvePySigTypes(pt, mod, scope, od, ad->u.sa, TRUE);

        /*
         * Note signal arguments are restricted in their types because we don't
         * (yet) support handwritten code for them.
         */
        if (issignal)
        {
            if (!supportedType(scope,od,ad,FALSE))
            {
                fatalStart();
                fprintf(stderr, "%s:%d: ", od->sloc.name, od->sloc.linenr);

                if (scope != NULL)
                {
                    fatalScopedName(classFQCName(scope));
                    fprintf(stderr, "::");
                }

                fatal("%s() argument %d has an unsupported type for a Python signature\n", od->cppname, a + 1);
            }
        }
        else if (!supportedType(scope, od, ad, TRUE))
        {
            fatalStart();

            if (od->sloc.name != NULL)
                fprintf(stderr, "%s:%d: ", od->sloc.name, od->sloc.linenr);

            if (scope != NULL)
            {
                fatalScopedName(classFQCName(scope));
                fprintf(stderr, "::");
            }

            if (isVirtual(od))
                fatal("%s() argument %d has an unsupported type for a Python signature - provide a valid type, %%MethodCode, %%VirtualCatcherCode and a C++ signature\n", od->cppname, a + 1);

            fatal("%s() argument %d has an unsupported type for a Python signature - provide a valid type, %%MethodCode and a C++ signature\n", od->cppname, a + 1);
        }

        if (scope != NULL)
            scopeDefaultValue(pt,scope,ad);
    }
}


/*
 * Resolve the type of a variable.
 */
static void resolveVariableType(sipSpec *pt, varDef *vd)
{
    int bad = TRUE;
    argDef *vtype = &vd->type;

    resolveType(pt, vd->module, vd->ecd, vtype, FALSE);

    switch (vtype->atype)
    {
    case mapped_type:
    case class_type:
        /* Class, Class & and Class * are supported. */

        if (vtype->nrderefs <= 1)
            bad = FALSE;
        break;

    case ascii_string_type:
    case latin1_string_type:
    case utf8_string_type:
    case sstring_type:
    case ustring_type:
    case string_type:
    case wstring_type:
        /*
         * (signed/unsigned) char, (signed/unsigned) char *, wchar_t, wchar_t *
         * are supported.
         */

        if (!isReference(vtype) && vtype->nrderefs <= 1)
            bad = FALSE;
        break;

    case cfloat_type:
    case float_type:
    case cdouble_type:
    case double_type:
    case enum_type:
    case bool_type:
    case cbool_type:
    case byte_type:
    case sbyte_type:
    case ubyte_type:
    case ushort_type:
    case short_type:
    case uint_type:
    case cint_type:
    case int_type:
    case ulong_type:
    case long_type:
    case ulonglong_type:
    case longlong_type:
    case ssize_type:
    case size_type:
    case pyobject_type:
    case pytuple_type:
    case pylist_type:
    case pydict_type:
    case pycallable_type:
    case pyslice_type:
    case pytype_type:
    case pybuffer_type:
    case capsule_type:
        /* These are supported without pointers or references. */

        if (!isReference(vtype) && vtype->nrderefs == 0)
            bad = FALSE;
        break;

    case struct_type:
    case void_type:
        /* A simple pointer is supported. */

        if (!isReference(vtype) && vtype->nrderefs == 1)
            bad = FALSE;
        break;

    /* Suppress a compiler warning. */
    default:
        ;
    }

    if (bad && (vd->getcode == NULL || (!noSetter(vd) && vd->setcode == NULL)))
    {
        fatalScopedName(vd->fqcname);
        fatal(" has an unsupported type - provide %%GetCode");

        if (!noSetter(vd))
            fatal(" and %%SetCode");

        fatal("\n");
    }
 
    if (vtype->atype != class_type && vd->accessfunc != NULL)
    {
        fatalScopedName(vd->fqcname);
        fatal(" has %%AccessCode but isn't a class instance\n");
    }

    if (vd->ecd != NULL)
        ifaceFileIsUsed(&vd->ecd->iff->used, vtype, FALSE);
    else
        ifaceFileIsUsed(&vd->module->used, vtype, FALSE);

    /* Scoped variables need a handler unless they have %AccessCode. */
    if (pyScope(vd->ecd) != NULL && vd->accessfunc == NULL)
    {
        setNeedsHandler(vd);
        setHasVarHandlers(vd->ecd);
    }
}


/*
 * See if a type is supported by the generated code.
 */
static int supportedType(classDef *cd,overDef *od,argDef *ad,int outputs)
{
    switch (ad -> atype)
    {
    case anyslot_type:
        /*
         * This must be an input, and must also have handwritten code.
         */

        ensureInput(cd,od,ad);
        return FALSE;

    case signal_type:
    case slot_type:
    case rxcon_type:
    case rxdis_type:
    case slotcon_type:
    case slotdis_type:
    case qobject_type:
    case ellipsis_type:
        /* These can only appear in argument lists without * or &. */

        ensureInput(cd,od,ad);
        return TRUE;

    case ascii_string_type:
    case latin1_string_type:
    case utf8_string_type:
    case sstring_type:
    case ustring_type:
    case string_type:
    case wstring_type:
        if (isReference(ad))
        {
            if (outputs && ad -> nrderefs <= 1)
            {
                defaultOutput(ad);
                return TRUE;
            }
        }
        else if (ad -> nrderefs == 0)
        {
            ensureInput(cd,od,ad);
            return TRUE;
        }
        else if (ad -> nrderefs == 1)
        {
            if (outputs)
                defaultInput(ad);
            else
                ensureInput(cd,od,ad);

            return TRUE;
        }
        else if (ad -> nrderefs == 2 && outputs)
        {
            defaultOutput(ad);
            return TRUE;
        }

        break;

    case cfloat_type:
    case float_type:
    case cdouble_type:
    case double_type:
    case enum_type:
    case bool_type:
    case cbool_type:
    case byte_type:
    case sbyte_type:
    case ubyte_type:
    case ushort_type:
    case short_type:
    case uint_type:
    case cint_type:
    case int_type:
    case ulong_type:
    case long_type:
    case ulonglong_type:
    case longlong_type:
    case ssize_type:
    case size_type:
    case pyobject_type:
    case pytuple_type:
    case pylist_type:
    case pydict_type:
    case pycallable_type:
    case pyslice_type:
    case pytype_type:
    case pybuffer_type:
    case capsule_type:
        if (isReference(ad))
        {
            if (isConstArg(ad))
            {
                ensureInput(cd, od, ad);
                return TRUE;
            }

            if (ad -> nrderefs == 0 && outputs)
            {
                defaultOutput(ad);
                return TRUE;
            }
        }
        else if (ad -> nrderefs == 0)
        {
            ensureInput(cd,od,ad);
            return TRUE;
        }
        else if (ad -> nrderefs == 1 && outputs)
        {
            defaultOutput(ad);
            return TRUE;
        }

        break;

    case mapped_type:
    case class_type:
        if (isReference(ad))
        {
            if (ad -> nrderefs == 0)
            {
                defaultInput(ad);
                return TRUE;
            }
            else if (ad -> nrderefs == 1 && outputs)
            {
                defaultOutput(ad);
                return TRUE;
            }
        }
        else if (ad -> nrderefs == 0)
        {
            ensureInput(cd,od,ad);
            return TRUE;
        }
        else if (ad -> nrderefs == 1)
        {
            if (outputs)
                defaultInput(ad);
            else
                ensureInput(cd,od,ad);

            return TRUE;
        }
        else if (ad -> nrderefs == 2 && outputs)
        {
            defaultOutput(ad);
            return TRUE;
        }

        break;

    case struct_type:
    case void_type:
        if (isReference(ad))
        {
            if (ad -> nrderefs == 1 && outputs)
            {
                defaultOutput(ad);
                return TRUE;
            }
        }
        else if (ad -> nrderefs == 1)
        {
            ensureInput(cd,od,ad);
            return TRUE;
        }
        else if (ad -> nrderefs == 2 && outputs)
        {
            defaultOutput(ad);
            return TRUE;
        }

        break;

    /* Suppress a compiler warning. */
    default:
        ;
    }

    /* Unsupported if we got this far. */
    return FALSE;
}


/*
 * Ensure the direction of an argument is an input.
 */
static void ensureInput(classDef *cd,overDef *od,argDef *ad)
{
    if (isOutArg(ad))
    {
        fatalStart();

        if (cd != NULL)
        {
            fatalScopedName(classFQCName(cd));
            fprintf(stderr, "::");
        }

        if (od != NULL)
            fprintf(stderr, "%s", od->cppname);

        fatal("() invalid argument type for /Out/\n");
    }

    setIsInArg(ad);
}


/*
 * Default the direction of an argument to an input.
 */
static void defaultInput(argDef *ad)
{
    if (!isInArg(ad) && !isOutArg(ad))
        setIsInArg(ad);
}


/*
 * Default the direction of an argument to an output unless the argument is
 * const.
 */
static void defaultOutput(argDef *ad)
{
    if (!isOutArg(ad) && !isInArg(ad))
    {
        if (isConstArg(ad))
            setIsInArg(ad);
        else
            setIsOutArg(ad);
    }
}


/*
 * Put a scoped name to stderr.
 */
void fatalScopedName(scopedNameDef *snd)
{
    fatalStart();

    while (snd != NULL)
    {
        fprintf(stderr, "%s", snd->name);

        snd = snd -> next;

        if (snd != NULL)
            fprintf(stderr, "::");
    }
}


/*
 * Compare two overloads and return TRUE if they are the same.
 */
static int sameCppOverload(overDef *od1, overDef *od2)
{
    /* They must both be enabled for the same API. */
    if (od1->api_range != od2->api_range)
        return FALSE;

    /* They must both be const, or both not. */
    if (isConst(od1) != isConst(od2))
        return FALSE;

    return sameSignature(od1->cppsig, od2->cppsig, TRUE);
}


/*
 * Compare two signatures and return TRUE if they are the same.
 */
int sameSignature(signatureDef *sd1, signatureDef *sd2, int strict)
{
    int a;

    if (strict)
    {
        /* The number of arguments must be the same. */
        if (sd1 -> nrArgs != sd2 -> nrArgs)
            return FALSE;
    }
    else
    {
        int na1, na2;

        /* We only count the compulsory arguments. */
        na1 = 0;

        for (a = 0; a < sd1 -> nrArgs; ++a)
        {
            if (sd1 -> args[a].defval != NULL)
                break;

            ++na1;
        }

        na2 = 0;

        for (a = 0; a < sd2 -> nrArgs; ++a)
        {
            if (sd2 -> args[a].defval != NULL)
                break;

            ++na2;
        }

        if (na1 != na2)
            return FALSE;
    }

    /* The arguments must be the same. */
    for (a = 0; a < sd1 -> nrArgs; ++a)
    {
        if (!strict && sd1 -> args[a].defval != NULL)
            break;

        if (!sameArgType(&sd1 -> args[a],&sd2 -> args[a],strict))
            return FALSE;
    }

    /* Must be the same if we've got this far. */
    return TRUE;
}


#define pyAsString(t)   ((t) == ustring_type || (t) == sstring_type || \
            (t) == string_type || (t) == ascii_string_type || \
            (t) == latin1_string_type || (t) == utf8_string_type)
#define pyAsFloat(t)    ((t) == cfloat_type || (t) == float_type || \
            (t) == cdouble_type || (t) == double_type)
#define pyAsInt(t)  ((t) == bool_type || (t) == ssize_type || \
            (t) == size_type || (t) == byte_type || (t) == sbyte_type || \
            (t) == ubyte_type || (t) == short_type || (t) == ushort_type || \
            (t) == cint_type || (t) == int_type || (t) == uint_type)
#define pyAsLong(t) ((t) == long_type || (t) == longlong_type)
#define pyAsULong(t)    ((t) == ulong_type || (t) == ulonglong_type)
#define pyAsAuto(t) ((t) == bool_type || \
            (t) == byte_type || (t) == sbyte_type || (t) == ubyte_type || \
            (t) == short_type || (t) == ushort_type || \
            (t) == int_type || (t) == uint_type || \
            (t) == float_type || (t) == double_type)
#define pyIsConstrained(t)  ((t) == cbool_type || (t) == cint_type || \
            (t) == cfloat_type || (t) == cdouble_type)

/*
 * Compare two argument types and return TRUE if they are the same.  "strict"
 * means as C++ would see it, rather than Python.
 */
static int sameArgType(argDef *a1, argDef *a2, int strict)
{
    /* The references must be the same. */
    if (isReference(a1) != isReference(a2) || a1->nrderefs != a2->nrderefs)
        return FALSE;

    if (strict)
    {
        /* The const should be the same. */
        if (isConstArg(a1) != isConstArg(a2))
            return FALSE;

        return sameBaseType(a1,a2);
    }

    /* If both are constrained fundamental types then the types must match. */
    if (pyIsConstrained(a1->atype) && pyIsConstrained(a2->atype))
        return (a1->atype == a2->atype);

    /* An unconstrained enum also acts as a (very) constrained int. */
    if ((pyAsInt(a1->atype) && a2->atype == enum_type && !isConstrained(a2)) ||
        (a1->atype == enum_type && !isConstrained(a1) && pyAsInt(a2->atype)))
        return TRUE;

    /* Python will see all these as strings. */
    if (pyAsString(a1->atype) && pyAsString(a2->atype))
        return TRUE;

    /* Python will see all these as floats. */
    if (pyAsFloat(a1->atype) && pyAsFloat(a2->atype))
        return TRUE;

    /* Python will see all these as ints. */
    if (pyAsInt(a1->atype) && pyAsInt(a2->atype))
        return TRUE;

    /* Python will see all these as longs. */
    if (pyAsLong(a1->atype) && pyAsLong(a2->atype))
        return TRUE;

    /* Python will see all these as unsigned longs. */
    if (pyAsULong(a1->atype) && pyAsULong(a2->atype))
        return TRUE;

    /* Python will automatically convert between these. */
    if (pyAsAuto(a1->atype) && pyAsAuto(a2->atype))
        return TRUE;

    /* All the special cases have been handled. */
    return sameBaseType(a1, a2);
}


/*
 * Compare two basic types and return TRUE if they are the same.
 */
int sameBaseType(argDef *a1, argDef *a2)
{
    /* The types must be the same. */
    if (a1->atype != a2->atype)
    {
        /*
         * If we are comparing a template with those that have already been
         * used to instantiate a class or mapped type then we need to compare
         * with the class or mapped type name.
         */
        if (a1->atype == class_type && a2->atype == defined_type)
            return compareScopedNames(a1->u.cd->iff->fqcname, a2->u.snd) == 0;

        if (a1->atype == defined_type && a2->atype == class_type)
            return compareScopedNames(a2->u.cd->iff->fqcname, a1->u.snd) == 0;

        if (a1->atype == mapped_type && a2->atype == defined_type)
            return compareScopedNames(a1->u.mtd->iff->fqcname, a2->u.snd) == 0;

        if (a1->atype == defined_type && a2->atype == mapped_type)
            return compareScopedNames(a2->u.mtd->iff->fqcname, a1->u.snd) == 0;

        if (a1->atype == enum_type && a2->atype == defined_type)
            return compareScopedNames(a1->u.ed->fqcname, a2->u.snd) == 0;

        if (a1->atype == defined_type && a2->atype == enum_type)
            return compareScopedNames(a2->u.ed->fqcname, a1->u.snd) == 0;

        return FALSE;
    }

    switch (a1->atype)
    {
    case class_type:
        if (a1->u.cd != a2->u.cd)
            return FALSE;

        break;

    case enum_type:
        if (a1->u.ed != a2->u.ed)
            return FALSE;

        break;

    case slotcon_type:
    case slotdis_type:
        if (!sameSignature(a1->u.sa, a2->u.sa, TRUE))
            return FALSE;

        break;

    case template_type:
        {
            int a;
            templateDef *td1, *td2;

            td1 = a1->u.td;
            td2 = a2->u.td;

            if (compareScopedNames(td1->fqname, td2->fqname) != 0 ||
                    td1->types.nrArgs != td2->types.nrArgs)
                return FALSE;

            for (a = 0; a < td1->types.nrArgs; ++a)
            {
                argDef *td1ad = &td1->types.args[a];
                argDef *td2ad = &td2->types.args[a];

                if (td1ad->nrderefs != td2ad->nrderefs)
                    return FALSE;

                if (!sameBaseType(td1ad, td2ad))
                    return FALSE;
            }

            break;
        }

    case struct_type:
        if (compareScopedNames(a1->u.sname, a2->u.sname) != 0)
            return FALSE;

        break;

    case defined_type:
        if (compareScopedNames(a1->u.snd, a2->u.snd) != 0)
            return FALSE;

        break;

    case mapped_type:
        if (a1->u.mtd != a2->u.mtd)
            return FALSE;

        break;

    /* Suppress a compiler warning. */
    default:
        ;
    }

    /* Must be the same if we've got this far. */
    return TRUE;
}


/*
 * See if two Python signatures are the same as far as Python is concerned.
 */
static int samePythonSignature(signatureDef *sd1, signatureDef *sd2)
{
    int a1, a2;

    a1 = a2 = -1;

    for (;;)
    {
        a1 = nextSignificantArg(sd1, a1);
        a2 = nextSignificantArg(sd2, a2);

        if (a1 < 0 || a2 < 0)
            break;

        if (!sameArgType(&sd1->args[a1], &sd2->args[a2], FALSE))
            return FALSE;
    }

    return (a1 < 0 && a2 < 0);
}


/*
 * Return the next significant argument from a Python signature (ie. one that
 * is not optional or an output only argument.  Return -1 if there isn't one.
 */
static int nextSignificantArg(signatureDef *sd, int a)
{
    while (++a < sd->nrArgs)
    {
        if (sd->args[a].defval != NULL)
            break;

        if (isInArg(&sd->args[a]))
            return a;
    }

    return -1;
}


/*
 * The equivalent of strcmp() for scoped names.
 */
int compareScopedNames(scopedNameDef *snd1, scopedNameDef *snd2)
{
    /* Strip the global scope if the target doesn't specify it. */
    if (snd2->name[0] != '\0')
        snd1 = removeGlobalScope(snd1);

    while (snd1 != NULL && snd2 != NULL)
    {
        int res = strcmp(snd1->name, snd2->name);

        if (res != 0)
            return res;

        snd1 = snd1->next;
        snd2 = snd2->next;
    }

    if (snd1 == NULL)
        return (snd2 == NULL ? 0 : -1);

    return 1;
}


/*
 * Add an explicit scope to the default value of an argument if possible.
 */

static void scopeDefaultValue(sipSpec *pt,classDef *cd,argDef *ad)
{
    valueDef *vd, **tailp, *newvd;

    /*
     * We do a quick check to see if we need to do anything.  This means
     * we can limit the times we need to copy the default value.  It needs
     * to be copied because it will be shared by class versions that have
     * been created on the fly and it may need to be scoped differently for
     * each of those versions.
     */

    for (vd = ad -> defval; vd != NULL; vd = vd -> next)
        if (vd -> vtype == scoped_value && vd -> u.vscp -> next == NULL)
            break;

    if (vd == NULL)
        return;

    /*
     * It's not certain that we will do anything, but we assume we will and
     * start copying.
     */

    newvd = NULL;
    tailp = &newvd;

    for (vd = ad -> defval; vd != NULL; vd = vd -> next)
    {
        mroDef *mro;
        scopedNameDef *origname;
        valueDef *new;

        /* Make the copy. */

        new = sipMalloc(sizeof (valueDef));

        *new = *vd;
        *tailp = new;
        tailp = &new -> next;

        /*
         * Skip this part of the expression if it isn't a named value
         * or it already has a scope.
         */

        if (vd -> vtype != scoped_value || vd -> u.vscp -> next != NULL)
            continue;

        /*
         * Search the class hierarchy for an enum value with the same
         * name.  If we don't find one, leave it as it is (the compiler
         * will find out if this is a problem).
         */

        origname = vd -> u.vscp;

        for (mro = cd -> mro; mro != NULL; mro = mro -> next)
        {
            enumDef *ed;

            for (ed = pt -> enums; ed != NULL; ed = ed -> next)
            {
                enumMemberDef *emd;

                if (ed -> ecd != mro -> cd)
                    continue;

                for (emd = ed -> members; emd != NULL; emd = emd -> next)
                    if (strcmp(emd -> cname,origname -> name) == 0)
                    {
                        scopedNameDef *snd;

                        /*
                         * Take the scope from the
                         * class that the enum was
                         * defined in.
                         */

                        snd = copyScopedName(mro -> cd -> iff -> fqcname);
                        appendScopedName(&snd,origname);

                        new -> u.vscp = snd;

                        /* Nothing more to do. */

                        break;
                    }

                if (emd != NULL)
                    break;
            }

            if (ed != NULL)
                break;
        }
    }

    ad -> defval = newvd;
}


/*
 * Resolve a type if possible.
 */
static void resolveType(sipSpec *pt, moduleDef *mod, classDef *c_scope,
        argDef *type, int allow_defined)
{
    /* Loop until we've got to a base type. */
    while (type->atype == defined_type)
    {
        scopedNameDef *snd = type->u.snd;

        type->atype = no_type;

        /*
         * Search the local scopes unless what we are looking for has an
         * explicit global scope.
         */
        if (snd->name[0] != '\0')
        {
            classDef *scope;

            for (scope = c_scope; scope != NULL; scope = scope->ecd)
            {
                if (scope->iff->type == class_iface)
                    searchClassScope(pt, scope, snd, type);
                else
                    searchScope(pt, scope, snd, type);

                if (type->atype != no_type)
                    break;
            }

            if (type->atype != no_type)
                break;
        }

        nameLookup(pt, mod, snd, type);

        if (type->atype != no_type)
            break;

        if (allow_defined)
        {
            type->atype = defined_type;
            return;
        }

        fatalNoDefinedType(snd);
    }

    /* Get the base type of any slot arguments. */
    if (type->atype == slotcon_type || type->atype == slotdis_type)
    {
        int sa;

        for (sa = 0; sa < type->u.sa->nrArgs; ++sa)
            resolveType(pt, mod, c_scope, &type->u.sa->args[sa], FALSE);
    }

    /* See if the type refers to an instantiated template. */
    resolveInstantiatedClassTemplate(pt, type);

    /* Replace the base type if it has been mapped. */
    if (type->atype == struct_type || type->atype == template_type)
    {
        searchMappedTypes(pt, mod, NULL, type);

        /*
         * If we still have a template then see if we need to automatically
         * instantiate it.
         */
        if (type->atype == template_type)
        {
            mappedTypeTmplDef *mtt;

            for (mtt = pt->mappedtypetemplates; mtt != NULL; mtt = mtt->next)
                if (compareScopedNames(mtt->mt->type.u.td->fqname, type->u.td->fqname) == 0 && sameTemplateSignature(&mtt->mt->type.u.td->types, &type->u.td->types, TRUE))
                {
                    instantiateMappedTypeTemplate(pt, mod, mtt, type);
                    break;
                }
        }
    }

    /*
     * If we are in the main module then mark any generated types as being
     * needed.
     */
    if (generatingCodeForModule(pt, mod))
        setNeededType(type);
}


/*
 * Specify that a generated type is needed.
 */
static void setNeededType(argDef *ad)
{
    switch (ad->atype)
    {
    case class_type:
        ad->u.cd->iff->first_alt->needed = TRUE;
        break;

    case mapped_type:
        ad->u.mtd->real->iff->first_alt->needed = TRUE;
        break;

    case enum_type:
        setNeedsEnum(ad->u.ed->first_alt);
        break;

    default:
        ;
    }
}


/*
 * Specify that a set of thrown exceptions are needed.
 */
static void setNeededExceptions(sipSpec *pt, moduleDef *mod,
        throwArgs *exceptions)
{
    if (generatingCodeForModule(pt, mod) && exceptions != NULL)
    {
        int i;

        for (i = 0; i < exceptions->nrArgs; ++i)
            setNeedsException(exceptions->args[i]);
    }
}


/*
 * Specify that an exception is needed.
 */
static void setNeedsException(exceptionDef *xd)
{
    if (xd->cd != NULL)
        xd->cd->iff->first_alt->needed = TRUE;
    else
        xd->needed = TRUE;
}


/*
 * If the type corresponds to a previously instantiated class template then
 * replace it with the class that was created.
 */
static void resolveInstantiatedClassTemplate(sipSpec *pt, argDef *type)
{
    int a;
    classDef *cd;
    templateDef *td;
    signatureDef *sd;

    if (type->atype != template_type)
        return;

    td = type->u.td;
    sd = &td->types;

    for (a = 0; a < sd->nrArgs; ++a)
        resolveInstantiatedClassTemplate(pt, &sd->args[a]);

    for (cd = pt->classes; cd != NULL; cd = cd->next)
        if (cd->td != NULL &&
            compareScopedNames(cd->td->fqname, td->fqname) == 0 &&
            sameSignature(&cd->td->types, sd, TRUE))
        {
            type->atype = class_type;
            type->u.cd = cd;

            break;
        }
}


/*
 * Instantiate a mapped type template.
 */
static void instantiateMappedTypeTemplate(sipSpec *pt, moduleDef *mod,
        mappedTypeTmplDef *mtt, argDef *type)
{
    scopedNameDef *type_names, *type_values;
    mappedTypeDef *mtd;

    type_names = type_values = NULL;
    appendTypeStrings(type->u.td->fqname, &mtt->mt->type.u.td->types, &type->u.td->types, &mtt->sig, &type_names, &type_values);

    mtd = allocMappedType(pt, type);

    if (generatingCodeForModule(pt, mod))
        setIsUsedName(mtd->cname);

    mtd->iff = findIfaceFile(pt, mod, encodedTemplateName(type->u.td),
            mappedtype_iface, NULL, type);
    mtd->iff->module = mod;

    mtd->mtflags = mtt->mt->mtflags;

    if (mtt->mt->doctype != NULL)
        mtd->doctype = templateString(mtt->mt->doctype, type_names,
                type_values);

    if (mtt->mt->typehint_in != NULL)
        mtd->typehint_in = newTypeHint(
                templateString(mtt->mt->typehint_in->raw_hint, type_names,
                        type_values));

    if (mtt->mt->typehint_out != NULL)
        mtd->typehint_out = newTypeHint(
                templateString(mtt->mt->typehint_out->raw_hint, type_names,
                        type_values));

    mtd->typehint_value = mtt->mt->typehint_value;

    appendCodeBlockList(&mtd->iff->hdrcode,
            templateCode(pt, &mtd->iff->used, mtt->mt->iff->hdrcode,
                    type_names, type_values));

    mtd->convfromcode = templateCode(pt, &mtd->iff->used,
            mtt->mt->convfromcode, type_names, type_values);
    mtd->convtocode = templateCode(pt, &mtd->iff->used, mtt->mt->convtocode,
            type_names, type_values);

    mtd->next = pt->mappedtypes;
    pt->mappedtypes = mtd;

    if (type_names != NULL)
        freeScopedName(type_names);

    if (type_values != NULL)
        freeScopedName(type_values);

    mtd = copyTemplateType(mtd, type);

    /* Replace the template with the mapped type. */
    type->atype = mapped_type;
    type->typehint_in = mtd->typehint_in;
    type->typehint_out = mtd->typehint_out;
    type->typehint_value = mtd->typehint_value;

    type->u.mtd = mtd;
}


/*
 * Return a string based on an original with names replaced by corresponding
 * values.
 */
char *templateString(const char *src, scopedNameDef *names,
        scopedNameDef *values)
{
    char *dst = sipStrdup(src);

    while (names != NULL && values != NULL)
    {
        char *cp, *vname = values->name;
        int vname_on_heap = FALSE;
        size_t name_len, value_len;

        /* Skip over any leading const. */
        if (strstr(vname, "const ") == vname)
            vname += 6;

        name_len = strlen(names->name);
        value_len = strlen(vname);

        /* Translate any C++ scoping to Python. */
        while ((cp = strstr(vname, "::")) != NULL)
        {
            char *new_vname = sipMalloc(value_len);
            size_t pos = cp - vname;

            memcpy(new_vname, vname, pos);
            new_vname[pos] = '.';
            strcpy(new_vname + pos + 1, cp + 2);

            if (vname != values->name)
                free(vname);

            vname = new_vname;
            vname_on_heap = TRUE;
            --value_len;
        }

        while ((cp = strstr(dst, names->name)) != NULL)
        {
            char *new_dst = sipMalloc(strlen(dst) - name_len + value_len + 1);

            memcpy(new_dst, dst, cp - dst);
            memcpy(new_dst + (cp - dst), vname, value_len);
            strcpy(new_dst + (cp - dst) + value_len, cp + name_len);

            free(dst);
            dst = new_dst;
        }

        if (vname_on_heap)
            free(vname);

        names = names->next;
        values = values->next;
    }

    return dst;
}


/*
 * Search for a name in a class scope and return the corresponding type.
 */
static void searchClassScope(sipSpec *pt, classDef *c_scope,
        scopedNameDef *snd, argDef *ad)
{
    mroDef *mro;

    for (mro = c_scope->mro; mro != NULL; mro = mro->next)
    {
        searchScope(pt, mro->cd, snd, ad);

        if (ad->atype != no_type)
            break;
    }
}


/*
 * Search for a name in a scope and return the corresponding type.
 */
static void searchScope(sipSpec *pt, classDef *scope, scopedNameDef *snd,
        argDef *ad)
{
    scopedNameDef *tmpsnd;

    /* Append the name to the scope and see if it exists. */
    tmpsnd = copyScopedName(classFQCName(scope));
    appendScopedName(&tmpsnd, copyScopedName(snd));

    nameLookup(pt, scope->iff->module, tmpsnd, ad);

    freeScopedName(tmpsnd);
}


/*
 * Look up a name and return the corresponding type.
 */
static void nameLookup(sipSpec *pt, moduleDef *context, scopedNameDef *snd,
        argDef *ad)
{
    searchMappedTypes(pt, context, snd, ad);
    if (ad->atype != no_type)
        return;

    searchTypedefs(pt, snd, ad);
    if (ad->atype != no_type)
        return;

    searchEnums(pt, snd, ad);
    if (ad->atype != no_type)
        return;

    searchClasses(pt, context, snd, ad);
}


/*
 * Search the mapped types for a name and return the type.
 */
static void searchMappedTypes(sipSpec *pt, moduleDef *context,
        scopedNameDef *snd, argDef *ad)
{
    mappedTypeDef *mtd;
    scopedNameDef *oname;

    /* Patch back to defined types so we can use sameBaseType(). */
    if (snd != NULL)
    {
        oname = ad->u.snd;
        ad->u.snd = snd;
        ad->atype = defined_type;
    }
    else
    {
        /* Avoid a compiler warning. */
        oname = NULL;
    }

    for (mtd = pt->mappedtypes; mtd != NULL; mtd = mtd->next)
        if (sameBaseType(&mtd->type, ad))
        {
            /*
             * If we are building a consolidated module and this mapped type is
             * defined in a different module then see if that other module is
             * in a different branch of the module hierarchy.
             */
            if (isConsolidated(pt->module) && context != mtd->iff->module)
            {
                moduleListDef *mld;

                for (mld = context->allimports; mld != NULL; mld = mld->next)
                    if (mld->module == mtd->iff->module)
                        break;

                /* If it's in a different branch then we ignore it. */
                if (mld == NULL)
                    continue;
            }

            mtd = copyTemplateType(mtd, ad);

            /* Copy the type. */
            ad->atype = mapped_type;
            ad->u.mtd = mtd;

            /*
             * Copy the type hints if nothing was specified for this particular
             * context.
             */
            if (ad->typehint_in == NULL)
                ad->typehint_in = mtd->typehint_in;

            if (ad->typehint_out == NULL)
                ad->typehint_out = mtd->typehint_out;

            if (ad->typehint_value == NULL)
                ad->typehint_value = mtd->typehint_value;

            return;
        }

    /* Restore because we didn't find anything. */
    if (snd != NULL)
    {
        ad->u.snd = oname;
        ad->atype = no_type;
    }
}


/*
 * If a mapped type is based on a template then create a copy that keeps the
 * original types of the template arguments.
 */
static mappedTypeDef *copyTemplateType(mappedTypeDef *mtd, argDef *ad)
{
    int a;
    signatureDef *src, *dst;
    mappedTypeDef *mtd_copy;

    /* There is no need to do anything for non-template types. */
    if (mtd->type.atype != template_type)
        return mtd;

    /* Retain the original types if there are any. */
    mtd_copy = mtd;
    src = &ad->u.td->types;
    dst = NULL;

    for (a = 0; a < src->nrArgs; ++a)
    {
        typedefDef *tdd = src->args[a].original_type;

        if (tdd != NULL)
        {
            /*
             * Create an appropriately deep copy now that we know it is needed
             * and if it hasn't already been done.
             */
            if (dst == NULL)
            {
                templateDef *td_copy;

                mtd_copy = sipMalloc(sizeof (mappedTypeDef));
                *mtd_copy = *mtd;

                td_copy = sipMalloc(sizeof (templateDef));
                *td_copy = *mtd->type.u.td;
                mtd_copy->type.u.td = td_copy;

                dst = &td_copy->types;
            }

            dst->args[a].original_type = tdd;
        }
    }

    return mtd_copy;
}


/*
 * Search the typedefs for a name and return the type.
 */
void searchTypedefs(sipSpec *pt, scopedNameDef *snd, argDef *ad)
{
    typedefDef *td;

    for (td = pt->typedefs; td != NULL; td = td->next)
    {
        int res = compareScopedNames(td->fqname, snd);

        if (res == 0)
        {
            int i;

            /* Copy the type. */
            ad->atype = td->type.atype;
            ad->argflags |= td->type.argflags;
            ad->doctype = td->type.doctype;
            ad->typehint_in = td->type.typehint_in;
            ad->typehint_out = td->type.typehint_out;
            ad->typehint_value = td->type.typehint_value;
            ad->u = td->type.u;

            for (i = 0; i < td->type.nrderefs; ++i)
            {
                if (ad->nrderefs >= MAX_NR_DEREFS - 1)
                    fatal("Internal error - increase the value of MAX_NR_DEREFS\n");

                ad->derefs[ad->nrderefs++] = td->type.derefs[i];
            }

            if (ad->original_type == NULL)
                ad->original_type = td;

            break;
        }

        /* The list is sorted so stop if we have gone too far. */
        if (res > 0)
            break;
    }
}


/*
 * Search the enums for a name and return the type.
 */
static void searchEnums(sipSpec *pt, scopedNameDef *snd, argDef *ad)
{
    enumDef *ed;

    for (ed = pt->enums; ed != NULL; ed = ed->next)
    {
        if (ed->fqcname == NULL)
            continue;

        if (compareScopedNames(ed->fqcname, snd) == 0)
        {
            ad->atype = enum_type;
            ad->u.ed = ed;

            break;
        }
    }
}


/*
 * Search the classes for one with a particular name and return it as a type.
 */
static void searchClasses(sipSpec *pt, moduleDef *context,
        scopedNameDef *cname, argDef *ad)
{
    classDef *cd;

    for (cd = pt->classes; cd != NULL; cd = cd->next)
    {
        /*
         * Ignore an external class unless it was declared in the same context
         * (ie. module) as the name is being used.
         */
        if (isExternal(cd) && cd->iff->module != context)
            continue;

        if (compareScopedNames(classFQCName(cd), cname) == 0)
        {
            ad->atype = class_type;
            ad->u.cd = cd;

            /*
             * Copy the type hints if nothing was specified for this particular
             * context.
             */
            if (ad->typehint_in == NULL)
                ad->typehint_in = cd->typehint_in;

            if (ad->typehint_out == NULL)
                ad->typehint_out = cd->typehint_out;

            if (ad->typehint_value == NULL)
                ad->typehint_value = cd->typehint_value;

            break;
        }
    }
}


/*
 * Print an error message describing an undefined type to stderr and terminate.
 */

static void fatalNoDefinedType(scopedNameDef *snd)
{
    fatalScopedName(snd);
    fatal(" is undefined\n");
}


/*
 * Make sure all interface files for a signature are used.
 */
static void ifaceFilesAreUsedBySignature(ifaceFileList **used,
        signatureDef *sd, int need_types)
{
    int a;

    ifaceFileIsUsed(used, &sd->result, need_types);

    for (a = 0; a < sd->nrArgs; ++a)
        ifaceFileIsUsed(used, &sd->args[a], need_types);
}


/*
 * Make sure all interface files for a function are used.
 */
static void ifaceFilesAreUsedByOverload(ifaceFileList **used, overDef *od,
        int need_types)
{
    throwArgs *ta;

    ifaceFilesAreUsedBySignature(used, &od->pysig, need_types);

    if (od->cppsig != &od->pysig)
        ifaceFilesAreUsedBySignature(used, od->cppsig, need_types);

    if ((ta = od->exceptions) != NULL)
    {
        int a;

        for (a = 0; a < ta->nrArgs; ++a)
        {
            exceptionDef *xd = ta->args[a];

            appendToIfaceFileList(used, xd->iff);

            if (need_types)
                setNeedsException(xd);
        }
    }
}


/*
 * If a type has an interface file then add it to the the given list of used
 * interface files so that the header file is #included in the generated code.
 */
static void ifaceFileIsUsed(ifaceFileList **used, argDef *ad, int need_types)
{
    ifaceFileDef *iff;

    if ((iff = getIfaceFile(ad)) != NULL)
    {
        appendToIfaceFileList(used, iff);

        /*
         * For mapped type templates we also need the template arguments.
         * These will be in the mapped type's used list (which itself will be
         * empty for non-template mapped types).
         */
        if (ad->atype == mapped_type)
        {
            ifaceFileList *iffl = iff->used;

            for (iffl = iff->used; iffl != NULL; iffl = iffl->next)
                appendToIfaceFileList(used, iffl->iff);
        }
    }

    if (need_types)
        setNeededType(ad);
}


/*
 * Return the interface file for an enum, or NULL if it doesn't have one.
 */
static ifaceFileDef *getIfaceFileForEnum(enumDef *ed)
{
    if (ed->fqcname != NULL)
    {
        if (ed->ecd != NULL)
            return ed->ecd->iff;

        if (ed->emtd != NULL)
            return ed->emtd->iff;
    }

    return NULL;
}


/*
 * Return the interface file for a type, or NULL if it doesn't have one.
 */
static ifaceFileDef *getIfaceFile(argDef *ad)
{
    ifaceFileDef *iff;

    switch (ad->atype)
    {
    case class_type:
        iff = ad->u.cd->iff;
        break;

    case mapped_type:
        iff = ad->u.mtd->iff;
        break;

    case enum_type:
        iff = getIfaceFileForEnum(ad->u.ed);
        break;

    default:
        iff = NULL;
    }

    return iff;
}


/*
 * Create the sorted array of numbered types for a module.  For the main module
 * this will be every type defined in the module.  For other modules this will
 * be every type needed by the main module.
 */
static void createSortedNumberedTypesTable(sipSpec *pt, moduleDef *mod)
{
    classDef *cd;
    mappedTypeDef *mtd;
    enumDef *ed;
    argDef *ad;
    int i;

    /* Count the how many types there are. */
    mod->nr_needed_types = 0;

    for (cd = pt->classes; cd != NULL; cd = cd->next)
    {
        if (cd->iff->module != mod)
            continue;

        if (cd->iff->first_alt != cd->iff)
            continue;

        if (generatingCodeForModule(pt, mod) || cd->iff->needed)
            if (!isHiddenNamespace(cd))
                mod->nr_needed_types++;
    }

    for (mtd = pt->mappedtypes; mtd != NULL; mtd = mtd->next)
    {
        if (mtd->iff->module != mod)
            continue;

        if (mtd->iff->first_alt != mtd->iff)
            continue;

        if (generatingCodeForModule(pt, mod) || mtd->iff->needed)
            mod->nr_needed_types++;
    }

    for (ed = pt->enums; ed != NULL; ed = ed->next)
    {
        if (ed->module != mod)
            continue;

        if (ed->fqcname == NULL)
            continue;

        if (ed->ecd != NULL && isTemplateClass(ed->ecd))
            continue;

        if (ed->first_alt != ed)
            continue;

        if (generatingCodeForModule(pt, mod) || needsEnum(ed))
            mod->nr_needed_types++;
    }

    if (mod->nr_needed_types == 0)
        return;

    /* Allocate and populate the table. */
    ad = mod->needed_types = sipCalloc(mod->nr_needed_types, sizeof (argDef));

    for (cd = pt->classes; cd != NULL; cd = cd->next)
    {
        if (cd->iff->module != mod)
            continue;

        if (cd->iff->first_alt != cd->iff)
            continue;

        if (generatingCodeForModule(pt, mod) || cd->iff->needed)
            if (!isHiddenNamespace(cd))
            {
                ad->atype = class_type;
                ad->u.cd = cd;
                ad->name = cd->iff->name;

                ++ad;
            }
    }

    for (mtd = pt->mappedtypes; mtd != NULL; mtd = mtd->next)
    {
        if (mtd->iff->module != mod)
            continue;

        if (mtd->iff->first_alt != mtd->iff)
            continue;

        if (generatingCodeForModule(pt, mod) || mtd->iff->needed)
        {
            ad->atype = mapped_type;
            ad->u.mtd = mtd;
            ad->name = mtd->cname;

            ++ad;
        }
    }

    for (ed = pt->enums; ed != NULL; ed = ed->next)
    {
        if (ed->module != mod)
            continue;

        if (ed->fqcname == NULL)
            continue;

        if (ed->ecd != NULL && isTemplateClass(ed->ecd))
            continue;

        if (ed->first_alt != ed)
            continue;

        if (generatingCodeForModule(pt, mod) || needsEnum(ed))
        {
            ad->atype = enum_type;
            ad->u.ed = ed;
            ad->name = ed->cname;

            ++ad;
        }
    }

    /* Sort the table and assign type numbers. */
    qsort(mod->needed_types, mod->nr_needed_types, sizeof (argDef),
            compareTypes);

    for (ad = mod->needed_types, i = 0; i < mod->nr_needed_types; ++i, ++ad)
    {
        switch (ad->atype)
        {
        case class_type:
            ad->u.cd->iff->ifacenr = i;

            /* If we find a class called QObject, assume it's Qt. */
            if (strcmp(ad->name->text, "QObject") == 0)
            {
                if (pt->qobject_cd != NULL)
                    fatal("QObject has been defined more than once\n");

                pt->qobject_cd = ad->u.cd;
            }

            break;

        case mapped_type:
            ad->u.mtd->iff->ifacenr = i;
            break;

        case enum_type:
            ad->u.ed->enumnr = i;
            break;

        /* Suppress a compiler warning. */
        default:
            ;
        }
    }
}


/*
 * The qsort helper to compare two generated C/C++ type names.
 */
static int compareTypes(const void *t1, const void *t2)
{
    return strcmp(((argDef *)t1)->name->text, ((argDef *)t2)->name->text);
}


/*
 * Return the fully qualified C/C++ name for a generated type.
 */
scopedNameDef *getFQCNameOfType(argDef *ad)
{
    scopedNameDef *snd;

    switch (ad->atype)
    {
    case class_type:
        snd = classFQCName(ad->u.cd);
        break;

    case mapped_type:
        snd = ad->u.mtd->iff->fqcname;
        break;

    case enum_type:
        snd = ad->u.ed->fqcname;
        break;

    default:
        /* Suppress a compiler warning. */
        snd = NULL;
    }

    return snd;
}


/*
 * Return TRUE if we are generating code for a module, ie. we are a component
 * of a consolidated module, or the main module where there is no consolidated
 * module.
 */
static int generatingCodeForModule(sipSpec *pt, moduleDef *mod)
{
    if (isConsolidated(pt->module))
        return (pt->module == mod->container);

    return (pt->module == mod);
}


/*
 * Check that any properties are valid.
 */
static void checkProperties(classDef *cd)
{
    propertyDef *pd;

    for (pd = cd->properties; pd != NULL; pd = pd->next)
    {
        if (findMethod(cd, pd->get) == NULL)
            fatal("Property %s.%s has no get method %s()\n", cd->pyname->text,
                    pd->name->text, pd->get);

        if (pd->set != NULL && findMethod(cd, pd->set) == NULL)
            fatal("Property %s.%s has no set method %s()\n", cd->pyname->text,
                    pd->name->text, pd->set);
    }
}


/*
 * Return the method of a class with a given name.
 */
memberDef *findMethod(classDef *cd, const char *name)
{
    memberDef *md;

    for (md = cd->members; md != NULL; md = md->next)
        if (strcmp(md->pyname->text, name) == 0)
            break;

    return md;
}
