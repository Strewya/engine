function format(fmt)
{
  var result = "";
  for(var i=0; i < fmt.length; ++i)
  {
    if(fmt[i] == '%')
    {
      var iArg = parseInt(fmt[i+1]);
      if(iArg < arguments.length)
      {
        result += arguments[iArg];
        ++i;
        continue;
      }
    }
    result += fmt[i];
  }
  return result;
}

function log(what)
{
  console.log(what);
}

function time(t)
{
  // assuming a valid time value expressed in game frames
  t = Math.max(0, t/Game.fps);
  if(t < 5)
  {
    // leave as is?
    t = t.toFixed(1)
  }
  else
  {
    t = Math.floor(t);
  }
  return t;
}

if(typeof MP === 'undefined')
{
  MP = {};
  MP.HookIndex = Game.customLogic.length;
  MP.OriginalUpdateMenu = Game.UpdateMenu;
}

function declare(obj, name, value, forceInit)
{
  if(typeof obj[name] === 'undefined' || forceInit)
  {
    obj[name] = value;
  }
}

MP.init = function(reset)
{
  // install/replace the hook
  Game.customLogic[MP.HookIndex] = MP.updateHook;
  Game.UpdateMenu = MP.update;
  
  // update frequency constants
  MP.Const = {};
  MP.Const.GameUpdateT = Game.fps * 5;
  MP.Const.UpdatesPerSecondFocused = 10;
  MP.Const.UpdateTFocused = Game.fps / MP.Const.UpdatesPerSecondFocused;
  MP.Const.UpdatesPerSecondUnfocused = 1;
  MP.Const.UpdateTUnfocused = Game.fps / MP.Const.UpdatesPerSecondUnfocused;
  
  // golden cookie timer vars - persistent
  declare(MP, 'GoldenCookieTimer', {}, reset);
  declare(MP.GoldenCookieTimer, 'Accumulator', 0, reset);
  declare(MP.GoldenCookieTimer, 'Count', 0, reset);
  declare(MP.GoldenCookieTimer, 'Min', 0, reset);
  declare(MP.GoldenCookieTimer, 'Max', 0, reset);
  // transient, but require init
  MP.GoldenCookieTimer.Chain = false;
  MP.GoldenCookieTimer.Spawned = false;
  MP.GoldenCookieTimer.SoundPlayed = false;
  MP.GoldenCookieTimer.Previous = 0;
}

MP.updateHook = function()
{
  var UpdateT = document.hasFocus() || Game.prefs.focus ? MP.Const.UpdateTFocused : MP.Const.UpdateTUnfocused;
  var GameWillUpdateMenu = (Game.T%MP.Const.GameUpdateT == 0 && !Game.mouseDown);
  var MPMustUpdateMenu = (Game.T%UpdateT == 0 && !Game.mouseDown);
  // check if i need to update the menu manually, or the game handles it
  if(!GameWillUpdateMenu && MPMustUpdateMenu)
  {
    MP.update();
  }
}

MP.update = function()
{
  MP.OriginalUpdateMenu();
  // add additional stuff
  MP.updateGoldenCookieTimes();
  MP.updateFrenzyEffects();
  MP.updateLuckyBankRequirements();
  MP.updateChainBankRequirements();
  MP.updateWrinklerReward();
  MP.updatePrestigeLevels(); // should be done after wrinkler update since it uses the wrinkler reward for calculation
  MP.writeMoreStats();
}

function checkTimerDisplay(Time, Text)
{
  if(Time > MP.GoldenCookieTimer.Time)
  {
    MP.GoldenCookieTimer.Text = Text;
    MP.GoldenCookieTimer.TimeUntil = time(Time - MP.GoldenCookieTimer.Time);
    return true;
  }
  return false;
}

MP.updateGoldenCookieTimes = function()
{
  MP.GoldenCookieTimer.TimeUntil = 0;
  
  if(Game.cookieClicks == 0)
  { 
    MP.init(true);
  }
  
  // updates until the cookie spawns
  if(Game.goldenCookie.life <= 0)
  {
    // since cookie has not spawned yet, reset some flags
    MP.GoldenCookieTimer.SoundPlayed = false;
    MP.GoldenCookieTimer.Spawned = false;
    // remember the time so we can add it to accumulator for calculating average spawn time
    MP.GoldenCookieTimer.Time = Game.goldenCookie.time;
    // calc average time
    var AvgRecorded = (MP.GoldenCookieTimer.Count > 0) ? MP.GoldenCookieTimer.Accumulator / MP.GoldenCookieTimer.Count : 0;
    var LastRecorded = MP.GoldenCookieTimer.Previous;
    
    var List = [
      {Time:Game.goldenCookie.minTime, Text:"threshold"},
      {Time:MP.GoldenCookieTimer.Min, Text:"minimum"},
      {Time:AvgRecorded, Text:"average"},
      {Time:LastRecorded, Text:"previous"},
      {Time:MP.GoldenCookieTimer.Max, Text:"maximum"},
      {Time:Game.goldenCookie.maxTime, Text:"upper limit"},
    ];
    if(AvgRecorded > LastRecorded)
    {
      var tmp = List[2];
      List[2] = List[3];
      List[3] = tmp;
    }
    
    for(var i = 0; i < List.length; ++i)
    {
      if(checkTimerDisplay(List[i].Time, List[i].Text)) return;
    }
  }
  else
  {
    if(MP.GoldenCookieTimer.SoundPlayed == false && Game.goldenCookie.chain == 0)
    {
      PlaySound('snd/tick.mp3', 100/Game.volume);
      MP.GoldenCookieTimer.SoundPlayed = true;
    }
    if(!MP.GoldenCookieTimer.Spawned)
    {
      MP.GoldenCookieTimer.Spawned = true;
      // ignore if this cookie is chain or previous was chain
      if(Game.goldenCookie.chain == 0 && MP.GoldenCookieTimer.Chain == 0)
      {
        // average data
        MP.GoldenCookieTimer.Accumulator += MP.GoldenCookieTimer.Time;
        MP.GoldenCookieTimer.Count += 1;
        // max data
        if(MP.GoldenCookieTimer.Max == 0 || MP.GoldenCookieTimer.Max < MP.GoldenCookieTimer.Time)
        {
          MP.GoldenCookieTimer.Max = MP.GoldenCookieTimer.Time;
        }
        // min data
        if(MP.GoldenCookieTimer.Min == 0 || MP.GoldenCookieTimer.Min > MP.GoldenCookieTimer.Time)
        {
          MP.GoldenCookieTimer.Min = MP.GoldenCookieTimer.Time;
        }
        // previous data
        MP.GoldenCookieTimer.Previous = MP.GoldenCookieTimer.Time;
      }
      MP.GoldenCookieTimer.Chain = Game.goldenCookie.chain;
    }
    
    MP.GoldenCookieTimer.Text = "remaining";
    MP.GoldenCookieTimer.TimeUntil = time(Game.goldenCookie.life);
  }
}

function getEffectName(Effect)
{
  for(var i = 0; i < Game.goldenCookie.choices.length/2; ++i)
  {
    if(Game.goldenCookie.choices[i*2+1] == Effect) return Game.goldenCookie.choices[i*2];
  }
  return "";
}

MP.updateFrenzyEffects = function()
{
  if(Game.frenzy > 0)
  {
    // there is a frenzy thing in effect
    if(Game.frenzy > MP.FrenzyTimer)
    {
      // only change the text if the game has a higher frenzy timer
      MP.FrenzyEffect = getEffectName(Game.goldenCookie.last);
    }
    MP.FrenzyTimer = Game.frenzy;
  }
  else
  {
    // no frenzy
    MP.FrenzyEffect = "None";
    MP.FrenzyTimer = 0;
  }
  
  if(Game.clickFrenzy > 0)
  {
    // there is a frenzy thing in effect
    if(Game.clickFrenzy > MP.ClickFrenzyTimer)
    {
      // only change the text if the game has a higher frenzy timer
      MP.ClickFrenzyEffect = getEffectName(Game.goldenCookie.last);
    }
    MP.ClickFrenzyTimer = Game.clickFrenzy;
  }
  else
  {
    // no frenzy
    MP.ClickFrenzyEffect = "None";
    MP.ClickFrenzyTimer = 0;
  } 
}

MP.getWrinklerCookies = function(Wrinkler)
{
  var Sucked = Wrinkler.sucked;
  var Multiplier = 1.1;
  if (Game.Has('Sacrilegious corruption')) Multiplier *= 1.05;
  if (Wrinkler.type == 1) Multiplier *= 3;//shiny wrinklers are an elusive, profitable breed
  Sucked *= Multiplier;
  if (Game.Has('Wrinklerspawn')) Sucked *= 1.05;
  return Sucked;
}

MP.updateWrinklerReward = function()
{
  MP.WrinklerReward = 0;
  for (var i in Game.wrinklers)
  {
    MP.WrinklerReward += MP.getWrinklerCookies(Game.wrinklers[i]);
  }
}

MP.updatePrestigeLevels = function()
{
  var ChipsOwned = Math.floor(Game.HowMuchPrestige(Game.cookiesReset));
  var ChipsGainWithoutWrinklersPopped = Math.floor(Game.HowMuchPrestige(Game.cookiesReset+Game.cookiesEarned)) - ChipsOwned;
  var ChipsGainWithWrinlersPopped = Math.floor(Game.HowMuchPrestige(Game.cookiesReset + Game.cookiesEarned + MP.WrinklerReward)) - ChipsOwned;
  MP.PrestigeLevelsTotal = ChipsGainWithWrinlersPopped;
  MP.PrestigeLevelsWrinkler = ChipsGainWithWrinlersPopped - ChipsGainWithoutWrinklersPopped;
}

MP.updateLuckyBankRequirements = function()
{
  var Bank = Game.cookies;
  var Cps = Game.cookiesPs;
  var Frenzy = Game.frenzy > 0;
  var CurrentFrenzyPower = 1;
  if(Frenzy) CurrentFrenzyPower = Game.frenzyPower;
  
  var BaseCps = Cps / CurrentFrenzyPower;
  var FrenzyMult = 7;
  
  MP.LuckyRewardBank = 13 + Math.floor(Bank * 0.15);
  MP.LuckyRewardBase = 13 + Math.floor(BaseCps * 900);
  MP.LuckyRewardFrenzy = 13 + Math.floor(BaseCps * FrenzyMult * 900);
  
  MP.MaxLuckyBankRequirementBase = Math.floor(MP.LuckyRewardBase/0.15 - 13);
  MP.MaxLuckyBankDiffBase = MP.MaxLuckyBankRequirementBase - Bank;
  MP.MaxLuckyBankTextBase = "missing";
  if(MP.MaxLuckyBankDiffBase < 0)
  {
    MP.MaxLuckyBankTextBase = "extra";
    MP.MaxLuckyBankDiffBase = -MP.MaxLuckyBankDiffBase;
  }
  
  MP.MaxLuckyBankRequirementFrenzy = Math.floor(MP.LuckyRewardFrenzy/0.15 - 13);
  MP.MaxLuckyBankDiffFrenzy = MP.MaxLuckyBankRequirementFrenzy - Bank;
  MP.MaxLuckyBankTextFrenzy = "missing";
  if(MP.MaxLuckyBankDiffFrenzy < 0)
  {
    MP.MaxLuckyBankTextFrenzy = "extra";
    MP.MaxLuckyBankDiffFrenzy = -MP.MaxLuckyBankDiffFrenzy;
  }
  
  MP.CurrentLuckyReward = Math.min(MP.LuckyRewardBank, Frenzy ? (MP.LuckyRewardFrenzy) : (MP.LuckyRewardBase));
}

MP.updateChainBankRequirements = function()
{
  var FrenzyPower = Game.frenzy > 0 ? Game.frenzyPower : 1;
  var Cps = Game.cookiesPs / FrenzyPower;
  
  var BankQuarter = Game.cookies * 0.25;
  var Cps6 = Cps * 6*60*60;
  var Cps42 = Cps6 * 7;
  // the chain will break if the next cookie is greater than any of these
  // so, in order to maximize the chain gain, the BANK needs to be greater than both of these outside of and during frenzy respectively
  MP.ChainBankBase = Cps6;
  MP.ChainBankFrenzy = Cps42;
  MP.ChainBankDiffBase = Cps6 - Game.cookies;
  MP.ChainBankDiffFrenzy = Cps42 - Game.cookies;
  MP.ChainBankTextBase = "missing";
  MP.ChainBankTextFrenzy = "missing";
  if(MP.ChainBankDiffBase < 0)
  {
    MP.ChainBankTextBase = "extra";
    MP.ChainBankDiffBase = -MP.ChainBankDiffBase;
  }
  if(MP.ChainBankDiffFrenzy < 0)
  {
    MP.ChainBankTextFrenzy = "extra";
    MP.ChainBankDiffFrenzy = -MP.ChainBankDiffFrenzy;
  }
}

function cookies(Amount)
{
  return format('<div class="price plain">%1</div>', Beautify(Amount));
}

function listing(Header, Text)
{
  return format('<div class="listing"><b>%1</b> %2</div>', Header, Text);
}

function timerText()
{
  var Spawned = MP.GoldenCookieTimer.Spawned ? "!!!SPAWNED!!!   " : "";  
  return format("%3%1 %2 sec", MP.GoldenCookieTimer.Text, MP.GoldenCookieTimer.TimeUntil, Spawned);
}

function button(Text, Callback)
{
  return format('<a class="option" %1="%2(); PlaySound(\'snd/tick.mp3\');">%3</a>', Game.clickStr, Callback, Text)
}

MP.writeMoreStats = function()
{
  if(Game.onMenu != 'stats') return;
    
  var Menu = l('menu');
  var Str = Menu.innerHTML;
  var LineBreak = '<br>';
  
  var NewStats = "";
  NewStats += listing('Lucky! cookies required:', format('%1 (%2 %3)', cookies(MP.MaxLuckyBankRequirementBase), MP.MaxLuckyBankTextBase, cookies(MP.MaxLuckyBankDiffBase)));
  NewStats += listing('Lucky! cookies required (Frenzy):', format('%1 (%2 %3)', cookies(MP.MaxLuckyBankRequirementFrenzy), MP.MaxLuckyBankTextFrenzy, cookies(MP.MaxLuckyBankDiffFrenzy)));
  NewStats += listing('Lucky! max reward:', cookies(MP.LuckyRewardBase));
  NewStats += listing('Lucky! max reward (Frenzy):', cookies(MP.LuckyRewardFrenzy));
  NewStats += listing('Lucky! reward:', cookies(MP.CurrentLuckyReward));
  NewStats += listing('Chain cookies required:', format('%1 (%2 %3)', cookies(MP.ChainBankBase), MP.ChainBankTextBase, cookies(MP.ChainBankDiffBase)));
  NewStats += listing('Chain cookies required (Frenzy):', format('%1 (%2 %3)', cookies(MP.ChainBankFrenzy), MP.ChainBankTextFrenzy, cookies(MP.ChainBankDiffFrenzy)));
  NewStats += LineBreak;
  NewStats += listing('Golden Cookie timer:', timerText());
  NewStats += listing('Frenzy effect:', format("%1 %2", MP.FrenzyEffect, MP.FrenzyTimer > 0 ? format(": %1 seconds left", time(MP.FrenzyTimer)) : ""));
  NewStats += listing('Click frenzy effect:', format("%1 %2", MP.ClickFrenzyEffect, MP.ClickFrenzyTimer > 0 ? format(": %1 seconds left", time(MP.ClickFrenzyTimer)) : ""));
  NewStats += LineBreak;
  NewStats += listing('Wrinkler reward if all are popped:', format("%1 %2", button("Pop all", "Game.CollectWrinklers"), cookies(MP.WrinklerReward)));
  NewStats += listing('Prestige levels gained after popping:', format("+%1%2", MP.PrestigeLevelsWrinkler, MP.PrestigeLevelsWrinkler > 0 ? format(" (total: +%1)", MP.PrestigeLevelsTotal) : ""));
  NewStats += LineBreak;
  NewStats += listing('Reindeer data:', 'TODO');
  
  
  NewStats = format('<div class="subsection"><div class="title">MarkoP mod</div>%1</div>', NewStats);
  
  var InsertHere = Str.indexOf('<div class="subsection"><div class="title">General</div>');
  var OldStats = Str.substring(0, InsertHere);
  var RestOfStats = Str.substring(InsertHere, Str.length);
  Menu.innerHTML = OldStats + NewStats + RestOfStats;
}

MP.writeMoreOptions = function()
{
  if(Game.onMenu != 'prefs') return;
  
  var Menu = l('menu');
  var Str = Menu.innerHTML;
  var LineBreak = '<br>';
  
  var NewPrefs = "";
  NewPrefs += Game.WriteButton();
  
  NewPrefs = format('<div class="subsection"><div class="title">MarkoP mod</div>%1</div>', NewPrefs);
  
  var InsertHere = Str.indexOf('<div class="subsection"><div class="title">General</div>');
  var OldStats = Str.substring(0, InsertHere);
  var RestOfStats = Str.substring(InsertHere, Str.length);
  Menu.innerHTML = OldStats + NewPrefs + RestOfStats;
}



// a function that adds options settings for:
//    frequency of menu updates
//    toggle debug info in stats panel

MP.init(false);
Game.Win('Third-party');
log('MarkoP Mod installed.');
Game.Notify("MarkoP mod installed", "best mod evah...", [16,5], 2);