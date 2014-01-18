// Teleport Terminal v0.2 by ssm2017 Binder & djphil (BY-NC-SA)

string url = "http://www.domaine-name.com";
string gui = "/grid/regions/guide";
integer debugger = FALSE;
integer text_hover = TRUE;
integer symbol_hover = TRUE;
vector default_color = <1.0, 1.0, 1.0>;
vector alert_color = <1.0, 0.0, 0.0>;
integer say_in_chat = TRUE;
integer link_number = 1;
integer texture_face = ALL_SIDES;
integer play_sound = TRUE;
integer set_textures = FALSE;
integer full_bright = TRUE;

// *********************************
//      STRINGS
// *********************************
// symbols
string _SYMBOL_RIGHT = "✔";
string _SYMBOL_WRONG = "✖";
string _SYMBOL_WARNING = "⚠";
string _SYMBOL_RESTART = "⟲";
string _SYMBOL_ARROW = "➜";
string _SYMBOL_NEXT = "► ► ►"; // ► ▷ ▸ ▹
string _SYMBOL_PREV = "◄ ◄ ◄"; // ◄ ◁ ◂ ◃
string _SYMBOL_MENU = "MENU"; // ▲ ▼ △ ▽ ▴ ▾ ▵ ▿
string _SYMBOL_HOR_BAR_1 = "⚌⚌⚌⚌⚌⚌⚌⚌⚌⚌⚌⚌⚌⚌⚌⚌⚌⚌⚌⚌";
string _SYMBOL_HOR_BAR_2 = "⚊⚊⚊⚊⚊⚊⚊⚊⚊⚊⚊⚊⚊⚊⚊⚊⚊⚊⚊⚊";

// common
string _THE_SCRIPT_WILL_STOP = "The script will stop";
string _CLIC_TO_RESTART = "Clic to restart";
string _RESET = "Reset";
string _CANCEL = "Cancel";
string _TO = "to";

// checks
string _MISSING_VAR_NAMED = "Missing var named";

// terminal
string _UPDATING_TERMINAL = "Updating terminal";
string _UPDATE_ERROR = "Update error";
string _EMPTY_TERMINAL = "Teleport Terminal is empty";
string _EMPTY_CATEGORY = "Empty category";
string _OFFLINE_REGION = "is currently offline";
string _IDENTICAL_REGION = "Identical region";

// http
string _WEBSITE = "Website";
string _SEE_WEBSITE = "Click to see the website";

// http errors
string _HTTP_ERROR = "http error";
string _REQUEST_TIMED_OUT = "Request timed out";
string _FORBIDDEN_ACCESS = "Forbidden access";
string _PAGE_NOT_FOUND = "Page not found";
string _INTERNET_EXPLODED = "the internet exploded!!";
string _SERVER_ERROR = "Server error";

// menu errors
string _ELAPSED_TIME = "Sorry, elapsed time";

// menu system
string _MAKE_A_CHOICE = "Make a choice";
string _BY_CATEGORY = "By category";
string _DEFINE_CATEGORY_CHOICE = "Select a category";
string _DEFINE_CATEGORY_00 = "00 => All Categories";
string _DEFINE_CATEGORY_01 = "01 => Official location";
string _DEFINE_CATEGORY_02 = "02 => Arts and culture";
string _DEFINE_CATEGORY_03 = "03 => Business";
string _DEFINE_CATEGORY_04 = "04 => Educationnal";
string _DEFINE_CATEGORY_05 = "05 => Gaming";
string _DEFINE_CATEGORY_06 = "06 => Hangout";
string _DEFINE_CATEGORY_07 = "07 => Newcomer friendly";
string _DEFINE_CATEGORY_08 = "08 => Parks and Nature";
string _DEFINE_CATEGORY_09 = "09 => Residential";
string _DEFINE_CATEGORY_10 = "10 => Shopping";
string _DEFINE_CATEGORY_11 = "11 => Other";
string _DEFINE_CATEGORY_12 = "12 => Rental";

// ============================================================
//      NOTHING SHOULD BE MODIFIED UNDER THIS LINE
// ============================================================
string ARGS_SEPARATOR = "||";
integer actual_region = 0;
integer category = 0;
// -1 or 0 All Categories
// 1 Official location
// 3 Artsand culture
// 4 Business
// 5 Educationnal
// 6 Gaming
// 7 Hangout
// 8 Newcomer friendly
// 9 Parks and Nature
// 10 Residential
// 11 Shopping
// 13 Other
// 14 Rental

string this_region;
string region_name;
string selected_category;
vector landing_coordinates;
vector landing_rotation;
integer online;
string  sound;

// fix OpenSim bug
integer PERMISSION_TELEPORT = 0x1000;

// *********************
//      FUNCTIONS
// *********************
// error
error(string message)
{
    llOwnerSay(_SYMBOL_WARNING + " " + message + ". " + _THE_SCRIPT_WILL_STOP + ".");   
    if (text_hover) llSetText(message, alert_color, 1.0);
    else llSetText("", default_color, 1.0);
}

// initialisation
textInitialisation()
{    
    if (symbol_hover)
    {
        llSetLinkPrimitiveParamsFast(2, [PRIM_TEXT, _SYMBOL_NEXT, default_color, 1.0]);
        llSetLinkPrimitiveParamsFast(3, [PRIM_TEXT, _SYMBOL_MENU, default_color, 1.0]);
        llSetLinkPrimitiveParamsFast(4, [PRIM_TEXT, _SYMBOL_PREV, default_color, 1.0]);
    }
    else llSetLinkPrimitiveParamsFast(LINK_SET, [PRIM_TEXT, "", <1.0, 1.0, 1.0>, 1.0]);
}

// call
key get_region_id;
getRegion(string way)
{
    if (debugger)
    {
        llOwnerSay(_SYMBOL_HOR_BAR_2);
        llOwnerSay(_SYMBOL_ARROW+ " "+ _UPDATING_TERMINAL + " " + _SYMBOL_RIGHT);
    }
    
    // sending values
    get_region_id = llHTTPRequest(url + "/metaverse-framework",
        [HTTP_METHOD, "POST", HTTP_MIMETYPE, "application/x-www-form-urlencoded"],
        "app=d4os_ui_regions_guide" +
        "&cmd=get_region" +
        "&output_type=message" +
        "&args_separator=" + ARGS_SEPARATOR +
        "&arg=" +
        "category=" + (string)category + ARGS_SEPARATOR + 
        "id=" + (string)actual_region + ARGS_SEPARATOR +
        "way=" + way
    );
}

// get server answer
getServerAnswer(integer status, string body)
{
    if (status == 499)
    {
        llOwnerSay(_SYMBOL_WARNING + " " + (string)status + " " + _REQUEST_TIMED_OUT);
    }

    else if (status == 403)
    {
        llOwnerSay(_SYMBOL_WARNING + " " + (string)status + " " + _FORBIDDEN_ACCESS);
    }

    else if (status == 404)
    {
        llOwnerSay(_SYMBOL_WARNING + " " + (string)status + " " + _PAGE_NOT_FOUND);
    }

    else if (status == 500)
    {
        llOwnerSay(_SYMBOL_WARNING + " " + (string)status + " " + _SERVER_ERROR);
    }

    else if (status != 403 && status != 404 && status != 500)
    {
        llOwnerSay(_SYMBOL_WARNING + " " + (string)status + " " + _INTERNET_EXPLODED);
        llOwnerSay(body);
    }
}

// display result
displayResult(string data)
{
    // data values: nid||region name||region texture||landing point||landing rotation||offset||total||online||description
    list values = llParseString2List(data, [ARGS_SEPARATOR], []);    
    actual_region = llList2Integer(values, 0);
    region_name = llList2String(values, 1);
    key texture = llList2Key(values, 2);
    landing_coordinates = (vector)llUnescapeURL(llList2String(values, 3));
    landing_rotation = (vector)llUnescapeURL(llList2String(values, 4));
    integer offset = llList2Integer(values, 5);
    integer total = llList2Integer(values, 6);
    online = llList2Integer(values, 7);

    llSetLinkTexture(link_number, texture, texture_face);
    string clean_selected_category = llGetSubString(selected_category, 6, -1);
    string text;

    if (actual_region == 0)
    {
        text = clean_selected_category + "\n[ " + (string)offset + "/" + (string)total + " ] " + _SYMBOL_ARROW + " " + _EMPTY_CATEGORY;
    }
    else
    {
        text = clean_selected_category + "\n[ " + (string)(offset + 1) + "/" + (string)total + " ] " + _SYMBOL_ARROW + " " + region_name;
    }

    if (say_in_chat)
    {
        llSay(PUBLIC_CHANNEL, text);
    }
        
    if (text_hover)
    {
        llSetText(text, default_color, 1.0);
    }
    else
    {
        llSetText("", default_color, 1.0);
    }

    if (actual_region == 0)
    {
        llResetScript();
    }

    if (debugger)
    {
        llOwnerSay("actual_region = " + (string)actual_region);
        llOwnerSay("region_name = " + region_name);
        llOwnerSay("texture = " + (string)texture);
        llOwnerSay("landing_coordinates = " + (string)landing_coordinates);
        llOwnerSay("landing_rotation = " + (string)landing_rotation);
        llOwnerSay("offset = " + (string)offset);
        llOwnerSay("total = " + (string)total);
        llOwnerSay("online = " + (string)online);
    }
}

// menu
list buttons_main;
list buttons_cat;
key avatar_uuid;
integer dialog_channel;
integer listen_handle;
string choices_main;
string choices_cat;

menuInitialisation() {
    buttons_main = [_RESET, _CANCEL, _WEBSITE, llGetSubString(_DEFINE_CATEGORY_00, 6, -1), _BY_CATEGORY];
    buttons_cat = ["10", "11", "12", "07", "08", "09", "04", "05", "06", "01", "02", "03"];
    choices_main = "\n" + _MAKE_A_CHOICE;
    choices_cat = "\n" + 
        _DEFINE_CATEGORY_CHOICE + "\n\n" +
        _DEFINE_CATEGORY_01 + "\n" +
        _DEFINE_CATEGORY_02 + "\n" +
        _DEFINE_CATEGORY_03 + "\n" +
        _DEFINE_CATEGORY_04 + "\n" +
        _DEFINE_CATEGORY_05 + "\n" +
        _DEFINE_CATEGORY_06 + "\n" +
        _DEFINE_CATEGORY_07 + "\n" +
        _DEFINE_CATEGORY_08 + "\n" +
        _DEFINE_CATEGORY_09 + "\n" +
        _DEFINE_CATEGORY_10 + "\n" +
        _DEFINE_CATEGORY_11 + "\n" +
        _DEFINE_CATEGORY_12;
}

// on change
onChange(integer change)
{
    if (change & CHANGED_INVENTORY)
    {
        llResetScript();
    }

    if (change & CHANGED_OWNER)
    {
        llResetScript();
    }
 
    if (change & CHANGED_LINK)
    {
        llResetScript();
    }
 
    if (change & CHANGED_REGION)
    {
        llResetScript();
    }
 
    if (change & CHANGED_REGION_START)
    {
        llResetScript();
    }
}

// ***********************
//  INIT PROGRAM
// ***********************
default
{
    on_rez(integer start_param)
    {
        llResetScript();
    }

    state_entry()
    {
        if (url == "")
        {
            llOwnerSay(_SYMBOL_WARNING + " " + _MISSING_VAR_NAMED + " \"url\"");
            state idle;
        }
        
        else
        {
            if (play_sound)
            {
                sound = llGetInventoryName(INVENTORY_SOUND, 0);
                    
                if (llGetInventoryKey(sound) == NULL_KEY)
                {
                    llOwnerSay("Son manquant dans l'inventaire " +  _SYMBOL_WRONG);
                    return;
                }
                else
                {
                    llPreloadSound(sound);
                }
            }
            dialog_channel = -1 - (integer)("0x" + llGetSubString( (string)llGetKey(), -7, -1));
            selected_category = _DEFINE_CATEGORY_00;
            this_region = llGetRegionName();
            textInitialisation();
            menuInitialisation();
            state run;
        }
    }
}

// **************
//      Run
// **************
state run
{
    on_rez(integer start_param)
    {
        llResetScript();
    }

    state_entry()
    {
        getRegion("start");
    }

    touch_start(integer num_detected)
    {
        if (play_sound)
        {
            llPlaySound(sound, 1.0);
        }
        llSetLinkPrimitiveParamsFast(llDetectedLinkNumber(0), [PRIM_FULLBRIGHT, ALL_SIDES, TRUE, PRIM_GLOW, ALL_SIDES, 0.05]);
    }

    touch_end(integer num_detected)
    {        
        string object_name = llGetLinkName(llDetectedLinkNumber(0));
        avatar_uuid = llDetectedKey(0);

        if (object_name == llGetLinkName(LINK_ROOT))
        {
            llSetLinkPrimitiveParamsFast(llDetectedLinkNumber(0), [PRIM_FULLBRIGHT, ALL_SIDES, full_bright, PRIM_GLOW, ALL_SIDES, 0.0]);

            if (online)
            {
                state teleport;
            }
            else
            {
                llInstantMessage(avatar_uuid, osKey2Name(avatar_uuid) + " " + region_name + " " + _OFFLINE_REGION);
            }
        }
        else
        {
            llSetLinkPrimitiveParamsFast(llDetectedLinkNumber(0), [PRIM_FULLBRIGHT, ALL_SIDES, FALSE, PRIM_GLOW, ALL_SIDES, 0.0]);
        }

        if (object_name == "menu")
        {
            state menu;
        }

        else if (object_name == "next")
        {
            getRegion("next");
        }
        
        else if (object_name == "prev")
        {
            getRegion("prev");
        }
    }

    http_response(key request_id, integer status, list metadata, string body)
    {
        if (debugger)
        {
            llOwnerSay("STATUS = " + (string)status + " META = " + llList2CSV(metadata) + " BODY = " + body);
        }
        
        if (request_id == get_region_id)
        {
            if (status != 200)
            {
                getServerAnswer(status, body);
            }

            else
            {
                if (body == "")
                {
                    llOwnerSay(_EMPTY_TERMINAL + " " + _SYMBOL_WRONG);
                    llOwnerSay(_SYMBOL_HOR_BAR_2);
                    return;
                }

                body = llStringTrim(body , STRING_TRIM);
                list data = llParseString2List(body, [";"], []);
                string result = llList2String(data, 0);

                if (result == "success")
                {
                    displayResult(llList2String(data, 1));
                }

                else
                {
                    llOwnerSay(body);
                    llOwnerSay(_SYMBOL_HOR_BAR_2);
                    error(_UPDATE_ERROR);
                    state idle;
                }
            }
        }
    }

    changed(integer change)
    {
        onChange(change);
    }
}

// **************
//      Menu
// **************
state menu
{
    on_rez(integer start_param)
    {
        llResetScript();
    }

    state_entry()
    {
        llListenRemove(listen_handle);
        listen_handle = llListen(dialog_channel, "", avatar_uuid, "");
        llDialog(avatar_uuid, choices_main, buttons_main, dialog_channel);
        llSetTimerEvent(10.0);
    }

    listen(integer channel, string name, key id, string message)
    {
        llListenRemove(listen_handle);
        integer valid = 0;

        if (message == _RESET)
        {
            llResetScript();
        }
        
        else if (message == _CANCEL)
        {
          valid = 1;
        }

        else if (message == _WEBSITE)
        {
            llSetTimerEvent(0.0);
            llLoadURL(avatar_uuid, _SEE_WEBSITE, url + gui);
            valid = 1;
        }

        else if (message == _BY_CATEGORY)
        {
            listen_handle = llListen(dialog_channel, "", avatar_uuid, "");
            llDialog(avatar_uuid, choices_cat, buttons_cat, dialog_channel);
            llSetTimerEvent(20.0);
        }

        else if (message == llGetSubString(_DEFINE_CATEGORY_00, 6, -1))
        {
            selected_category = _DEFINE_CATEGORY_00;
            category = 0;
            valid = 1;
        }

        else if (message == "01")
        {
            selected_category = _DEFINE_CATEGORY_01;
            category = 1;
            valid = 1;
        }
 
        else if (message == "02")
        {
            selected_category = _DEFINE_CATEGORY_02;
            category = 3;
            valid = 1;
        }

        else if (message == "03")
        {
            selected_category = _DEFINE_CATEGORY_03;
            category = 4;
            valid = 1;
        }
        
        else if (message == "04")
        {
            selected_category = _DEFINE_CATEGORY_04;
            category = 5;
            valid = 1;
        }
        
        else if (message == "05")
        {
            selected_category = _DEFINE_CATEGORY_05;
            category = 6;
            valid = 1;
        }
        
        else if (message == "06")
        {
            selected_category = _DEFINE_CATEGORY_06;
            category = 7;
            valid = 1;
        }
        
        else if (message == "07")
        {
            selected_category = _DEFINE_CATEGORY_07;
            category = 8;
            valid = 1;
        }
        
        else if (message == "08")
        {
            selected_category = _DEFINE_CATEGORY_08;
            category = 9;
            valid = 1;
        }
        
        else if (message == "09")
        {
            selected_category = _DEFINE_CATEGORY_09;
            category = 10;
            valid = 1;
        }
        
        else if (message == "10")
        {
            selected_category = _DEFINE_CATEGORY_10;
            category = 11;
            valid = 1;
        }
        
        else if (message == "11")
        {
            selected_category = _DEFINE_CATEGORY_11;
            category = 13;
            valid = 1;
        }
        
        else if (message == "12")
        {
            selected_category = _DEFINE_CATEGORY_12;
            category = 14;
            valid = 1;
        }
        
        else if (debugger)
        {
            llOwnerSay(_SYMBOL_HOR_BAR_2);
            llOwnerSay(message + " " + selected_category);
            llOwnerSay(_SYMBOL_HOR_BAR_1);
        }

        if (valid)
        {
            state run;
        }
    }

    timer()
    {
        llSetTimerEvent(0.0);
        llListenRemove(listen_handle);
        llSay(0, _SYMBOL_WARNING + " " + _ELAPSED_TIME);
        state run;
    }

    changed(integer change)
    {
        onChange(change);
    }

    state_exit()
    {
        llSetTimerEvent(0.0);
    }
}

// **************
//    Teleport
// **************
state teleport
{
    on_rez(integer start_param)
    {
        llResetScript();
    }

    state_entry()
    {
        if (region_name == this_region)
        {
            llInstantMessage(avatar_uuid, _SYMBOL_WARNING + " " + _IDENTICAL_REGION + " " + osKey2Name(avatar_uuid) + ". " + _MAKE_A_CHOICE + ".");
            state run;
        }
        else
        {
            llRequestPermissions(avatar_uuid, PERMISSION_TELEPORT);
        }
    }

    run_time_permissions(integer perm)
    {
        if (PERMISSION_TELEPORT & perm)
        {
            llTeleportAgent(avatar_uuid, region_name, landing_coordinates, landing_rotation);
            llInstantMessage(avatar_uuid, osKey2Name(avatar_uuid) + " " + _TO + ": " + region_name);
            llSetTimerEvent(2.0);
        }
        else
        {
            state run;
        }
    }

    timer()
    {
        llSetTimerEvent(0.0);
        state run;
    }

    changed(integer change)
    {
        onChange(change);
    }

    state_exit()
    {
        llSetTimerEvent(0.0);
    }
}

// **************
//      Error
// **************
state idle
{
    on_rez(integer start_param)
    {
        llResetScript();
    }

    state_entry()
    {
        llOwnerSay(_SYMBOL_RESTART + " " + _CLIC_TO_RESTART);
    }

    touch_start(integer num_detected)
    {
        llResetScript();
    }

    changed(integer change)
    {
        onChange(change);
    }
}
