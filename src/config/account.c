/*
 * account.c
 *
 * Copyright (C) 2012 - 2014 James Booth <boothj5@gmail.com>
 *
 * This file is part of Profanity.
 *
 * Profanity is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Profanity is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Profanity.  If not, see <http://www.gnu.org/licenses/>.
 *
 * In addition, as a special exception, the copyright holders give permission to
 * link the code of portions of this program with the OpenSSL library under
 * certain conditions as described in each individual source file, and
 * distribute linked combinations including the two.
 *
 * You must obey the GNU General Public License in all respects for all of the
 * code used other than OpenSSL. If you modify file(s) with this exception, you
 * may extend this exception to your version of the file(s), but you are not
 * obligated to do so. If you do not wish to do so, delete this exception
 * statement from your version. If you delete this exception statement from all
 * source files in the program, then also delete it here.
 *
 */

#include <stdlib.h>
#include <string.h>

#include <glib.h>

#include "jid.h"
#include "config/account.h"
#include "common.h"

ProfAccount*
account_new(const gchar * const name, const gchar * const jid,
    const gchar * const password, const gchar * eval_password, gboolean enabled, const gchar * const server,
    int port, const gchar * const resource, const gchar * const last_presence,
    const gchar * const login_presence, int priority_online, int priority_chat,
    int priority_away, int priority_xa, int priority_dnd,
    const gchar * const muc_service, const gchar * const muc_nick,
    const gchar * const otr_policy, GList *otr_manual, GList *otr_opportunistic,
    GList *otr_always)
{
    ProfAccount *new_account = malloc(sizeof(ProfAccount));

    new_account->name = strdup(name);

    if (jid != NULL) {
        new_account->jid = strdup(jid);
    } else {
        new_account->jid = strdup(name);
    }

    if (password != NULL) {
        new_account->password = strdup(password);
    } else {
        new_account->password = NULL;
    }

    if (eval_password != NULL) {
        new_account->eval_password = strdup(eval_password);
    } else {
        new_account->eval_password = NULL;
    }

    new_account->enabled = enabled;

    if (server != NULL) {
        new_account->server = strdup(server);
    } else {
        new_account->server = NULL;
    }

    if (resource != NULL) {
        new_account->resource = strdup(resource);
    } else {
        new_account->resource = NULL;
    }

    new_account->port = port;

    if (last_presence == NULL || !valid_resource_presence_string(last_presence)) {
        new_account->last_presence = strdup("online");
    } else {
        new_account->last_presence = strdup(last_presence);
    }

    if (login_presence == NULL) {
        new_account->login_presence = strdup("online");
    } else if (strcmp(login_presence, "last") == 0) {
        new_account->login_presence = strdup(login_presence);
    } else if (!valid_resource_presence_string(login_presence)) {
        new_account->login_presence = strdup("online");
    } else {
        new_account->login_presence = strdup(login_presence);
    }

    new_account->priority_online = priority_online;
    new_account->priority_chat = priority_chat;
    new_account->priority_away = priority_away;
    new_account->priority_xa = priority_xa;
    new_account->priority_dnd = priority_dnd;

    if (muc_service == NULL) {
        GString *g_muc_service = g_string_new("conference.");
        Jid *jidp = jid_create(new_account->jid);
        g_string_append(g_muc_service, jidp->domainpart);

        new_account->muc_service = g_muc_service->str;

        g_string_free(g_muc_service, FALSE);
        jid_destroy(jidp);
    } else {
        new_account->muc_service = strdup(muc_service);
    }

    if (muc_nick == NULL) {
        Jid *jidp = jid_create(new_account->jid);
        new_account->muc_nick = strdup(jidp->domainpart);
        jid_destroy(jidp);
    } else {
        new_account->muc_nick = strdup(muc_nick);
    }

    if (otr_policy != NULL) {
        new_account->otr_policy = strdup(otr_policy);
    } else {
        new_account->otr_policy = NULL;
    }

    new_account->otr_manual = otr_manual;
    new_account->otr_opportunistic = otr_opportunistic;
    new_account->otr_always = otr_always;

    return new_account;
}

char *
account_create_full_jid(ProfAccount *account)
{
    if (account->resource != NULL) {
        return create_fulljid(account->jid, account->resource);
    } else {
        return strdup(account->jid);
    }
}

void
account_free(ProfAccount *account)
{
    if (account != NULL) {
        free(account->name);
        free(account->jid);
        free(account->password);
        free(account->eval_password);
        free(account->resource);
        free(account->server);
        free(account->last_presence);
        free(account->login_presence);
        free(account->muc_service);
        free(account->muc_nick);
        free(account->otr_policy);
        g_list_free_full(account->otr_manual, g_free);
        g_list_free_full(account->otr_opportunistic, g_free);
        g_list_free_full(account->otr_always, g_free);
        free(account);
    }
}
