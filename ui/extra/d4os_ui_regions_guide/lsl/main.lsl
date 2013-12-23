// Teleport Terminal v0.1 for d4os_ui_regions_guide by ssm2017 Binder & djphil (BY-NC-SA)
 
string url = "http://www.domaine-name.com";
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
 
integer text_hover = TRUE;
vector text_color = <1.0, 1.0, 1.0>;
integer say_in_chat = TRUE;
integer texture_face = 0;

// ******************
//      STRINGS
// ******************
 
// checks
string _MISSING_VAR_NAMED = "Missing var named";
 
// terminal
string _EMPTY_CATEGORY = "Empty category";
string _OFFLINE_REGION = "is currently offline";

// teleport
string _TO = "to";
 
// http errors
string _HTTP_ERROR = "http error";
 
// ========================================================
//      NOTHING SHOULD BE MODIFIED UNDER THIS LINE
// ========================================================
string ARGS_SEPARATOR = "||";
integer actual_region = 0;
 
string this_region;
string region_name;
vector landing_coordinates;
vector landing_rotation;
integer online;
key avatarUUID;
 
// fix OpenSim bug (missing constant)
integer PERMISSION_TELEPORT = 0x1000;
 
// *********************
//      FUNCTIONS
// *********************
 
// call
key get_region_id;
getRegion(string way)
{
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
 
// display result
displayResult(string data)
{
    // data values : nid||region name||region texture||landing point||landing rotation||offset||total||online||description
    list values = llParseString2List(data, [ARGS_SEPARATOR], []);
    // set the actual region
    integer nid = llList2Integer(values, 0);
   
    if (nid == 0)
    {
        llSay(PUBLIC_CHANNEL, _EMPTY_CATEGORY);
        return;
    }
    else
    {
        actual_region = nid;
        region_name = llList2String(values, 1);
        key texture = llList2Key(values, 2);
        landing_coordinates = (vector)llUnescapeURL(llList2String(values, 3));
        landing_rotation = (vector)llUnescapeURL(llList2String(values, 4));
        integer offset = llList2Integer(values, 5);
        integer total = llList2Integer(values, 6);
        online = llList2Integer(values, 7);
       
        if (region_name != this_region)
        {
            llSetTexture(texture, texture_face);
           
            if (say_in_chat)
            {
                llSay(PUBLIC_CHANNEL, "[Destination " + (string)(offset + 1) + "/" + (string)total + "] " + region_name);
            }
       
            if (text_hover)
            {
                llSetText("[Destination " + (string)(offset + 1) + "/" + (string)total + "]\n" + region_name, text_color, 1.0);
            }
            else
            {
                llSetText("", text_color, 1.0);
            }
        }
    }
}
 
// teleport
list LastFewAgents;
PerformTeleport(key avatar)
{
    integer CurrentTime = llGetUnixTime();
    integer AgentIndex  = llListFindList(LastFewAgents, [avatar]);
   
    if (AgentIndex != -1)
    {
        integer PreviousTime = llList2Integer(LastFewAgents, AgentIndex + 1);
        if (PreviousTime >= (CurrentTime - 5)) return;
        LastFewAgents = llDeleteSubList(LastFewAgents, AgentIndex, AgentIndex + 1);
    }
 
    LastFewAgents += [avatar, CurrentTime];
   
    if (online)
    {
        llTeleportAgent(avatar, region_name, landing_coordinates, landing_rotation);
        llInstantMessage(avatar, osKey2Name(avatar) + " " + _TO + ": " + region_name);
    }
    else
    {
        llInstantMessage(avatar, osKey2Name(avatar) + " " + region_name + " " + _OFFLINE_REGION);
    }
}

// on change
onChange(integer change)
{
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
 
    if (change & 256)
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
        this_region = llGetRegionName();
       
        if (url == "")
        {
            llOwnerSay(_MISSING_VAR_NAMED + " \"url\"");
            state idle;
        }
        else
        {
            getRegion("start");
        }
    }
 
    touch_start(integer num_detected)
    {
        string object_name = llGetLinkName(llDetectedLinkNumber(0));
        avatarUUID = llDetectedKey(0);
       
        if (object_name == "next")
        {
            getRegion("next");
        }
        else if (object_name == "prev")
        {
            getRegion("prev");
        }
        else if (online)
        {
            llRequestPermissions(avatarUUID, PERMISSION_TELEPORT);
        }
        else
        {
            llInstantMessage(avatarUUID, osKey2Name(avatarUUID) + " " + region_name + " " + _OFFLINE_REGION);
        }
    }
 
    run_time_permissions(integer perm)
    {
        if (PERMISSION_TELEPORT & perm)
        {
            PerformTeleport(avatarUUID);
        }
    }
 
    http_response(key request_id, integer status, list metadata, string body)
    {    
        if (request_id == get_region_id)
        {
            if (status != 200)
            {
                llOwnerSay(_HTTP_ERROR);
            }
            else
            {
                body = llStringTrim(body , STRING_TRIM);
                list data = llParseString2List(body, [";"], []);
                string result = llList2String(data, 0);
 
                if (result == "success")
                {
                    displayResult(llList2String(data, 1));
                    return;
                }
               
                if (result == "error")
                {
                    llOwnerSay(llList2String(data, 1));
                }
                else
                {
                    llOwnerSay(body);
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
//      Error
// **************
state idle
{
    touch_start(integer num_detected)
    {
        llResetScript();
    }
 
    changed(integer change)
    {
        onChange(change);
    }
}
