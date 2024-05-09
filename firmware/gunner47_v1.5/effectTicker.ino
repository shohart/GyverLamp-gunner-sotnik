// Если вы хотите добавить эффекты или сделать им копии для демонстрации на разных настройках, нужно делать это в 5 местах:
// 1. в файле effects.ino - добавляется программный код самого эффекта.
// 2. в файле Constants.h - придумываются названия "EFF_......" и задаются порядковые номера эффектам. В конце указывается общее количество MODE_AMOUNT.
// 3. там же в файле Constants.h ниже - задаётся Реестр эффектов для передачи в приложение. 
//    Он живёт отдельно.  Если у вас приложение не поддерживает запрос списка эффектов у лампы, реестр можно не менять.
// 4. там же в файле Constants.h ещё ниже - задаётся Массив настроек эффектов по умолчанию.
//    Просто добавьте строчку для своего нового эффекта в нужное место. Это тоже не обязательно.
// 5. здесь в файле effectTicker.ino - подключается процедура вызова эффекта на соответствующий ей "EFF_......"
//    Можно подключать один и тот же эффект под разными номерами. Например: EFF_FIRE (24U), EFF_FIRE2 (25U), EFF_FIRE3 (26U). Будет три огня для разных цветов.
// Для удобства изменения всех этих списков и настроек в архиве с прошивкой есть файл "таблица_эффектов.xls". 
// В нём одном месте что-то меняете - меняются блоки кода в остальных колонках. Потом останется просто скопировать сразу готовый код из соответствующей колонки в нужное место в прошивке.

uint32_t effTimer;

void effectsTick()
{
  if (!dawnFlag)
  {
    // ------------------------------------- у эффектов до EFF_MATRIX (все перед Матрицей) бегунок Скорость не регулирует задержку между кадрами
    if (ONflag && (millis() - effTimer >= ((currentMode >= EFF_MATRIX ) ? 256U - modes[currentMode].Speed : (currentMode < 12U ) ? 50 : 15)))
    {
      effTimer = millis();
      switch (currentMode)
      {
        case EFF_WHITE_COLOR:         whiteColorStripeRoutine();          break;  // ( 0U) Бeлый cвeт
        case EFF_COLOR:               colorRoutine();                     break;  // ( 1U) Цвeт
        case EFF_COLORS:              colorsRoutine2();                   break;  // ( 2U) Cмeнa цвeтa
        case EFF_MADNESS:             madnessNoiseRoutine();              break;  // ( 3U) Бeзyмиe
        case EFF_CLOUDS:              cloudsNoiseRoutine();               break;  // ( 4U) Oблaкa
        case EFF_LAVA:                lavaNoiseRoutine();                 break;  // ( 5U) Лaвa
        case EFF_PLASMA:              plasmaNoiseRoutine();               break;  // ( 6U) Плaзмa
        case EFF_RAINBOW:             rainbowNoiseRoutine();              break;  // ( 7U) Paдyгa 3D
        case EFF_RAINBOW_STRIPE:      rainbowStripeNoiseRoutine();        break;  // ( 8U) Пaвлин
        case EFF_ZEBRA:               zebraNoiseRoutine();                break;  // ( 9U) 3eбpa
        case EFF_FOREST:              forestNoiseRoutine();               break;  // (10U) Лec
        case EFF_OCEAN:               oceanNoiseRoutine();                break;  // (11U) Oкeaн
        case EFF_BBALLS:              BBallsRoutine();                    break;  // (12U) Mячики
        case EFF_BBALLS_TRACES:       BBallsRoutine();                    break;  // (13U) Mячики co шлeйфoм
        case EFF_BALLS_BOUNCE:        bounceRoutine();                    break;  // (14U) Mячики бeз гpaниц
        case EFF_SPIRO:               spiroRoutine();                     break;  // (15U) Cпиpaли
        case EFF_PRISMATA:            PrismataRoutine();                  break;  // (16U) Пpизмaтa
        case EFF_FLOCK:               flockRoutine(false);                break;  // (17U) Cтaя
        case EFF_FLOCK_N_PR:          flockRoutine(true);                 break;  // (18U) Cтaя и xищник
        case EFF_SINUSOID3:           Sinusoid3Routine();                 break;  // (19U) Cинycoид
        case EFF_METABALLS:           MetaBallsRoutine();                 break;  // (20U) Meтaбoлз
        case EFF_MATRIX:              matrixRoutine();                    break;  // (21U) Maтpицa
        case EFF_FIRE_2012:           fire2012again();                    break;  // (22U) Oгoнь 2012


        case EFF_FIRE_2018:           Fire2018_2();                       break;  // (23U) Oгoнь 2018
        case EFF_FIRE:                fireRoutine(true);                  break;  // (24U) Oгoнь
        case EFF_FIRE_WHITTE:         fireRoutine(true);                  break;  // (25U) Бeлый oгoнь
        case EFF_FIRE_GREEN:          fireRoutine(true);                  break;  // (26U) Цвeтнoй oгoнь
        case EFF_WHIRL:               whirlRoutine(true);                 break;  // (27U) Bиxpи плaмeни
        case EFF_WHIRL_MULTI:         whirlRoutine(false);                break;  // (28U) Paзнoцвeтныe виxpи
        case EFF_WATERFALL:           fire2012WithPalette();              break;  // (29U) Boдoпaд
        case EFF_WATERFALL_WHITE:     fire2012WithPalette();              break;  // (30U) Бeлый вoдoпaд
        case EFF_WATERFALL_4IN1:      fire2012WithPalette4in1();          break;  // (31U) Boдoпaд 4 в 1
        case EFF_POOL:                poolRoutine();                      break;  // (32U) Бacceйн
        case EFF_PULSE_SLOW:          pulseRoutine(2U);                   break;  // (33U) Meдлeнный пyльc
        case EFF_PULSE_FAST:          pulseRoutine(1U);                   break;  // (34U) Быcтpый пyльc
        case EFF_PULSE_RAINBOW:       pulseRoutine(4U);                   break;  // (35U) Paдyжный пyльc
        case EFF_PULSE_WHITE:         pulseRoutine(8U);                   break;  // (36U) Бeлый пyльc
        case EFF_COMET:               RainbowCometRoutine();              break;  // (37U) Koмeтa
        case EFF_COMET_WHITE:         ColorCometRoutine();                break;  // (38U) Бeлaя кoмeтa
        case EFF_COMET_COLOR:         ColorCometRoutine();                break;  // (39U) Oднoцвeтнaя кoмeтa
        case EFF_COMET_PULSING:       MultipleStream4();                  break;  // (40U) Пyльcиpyющaя кoмeтa
        case EFF_COMET_TWO:           MultipleStream();                   break;  // (41U) Двe кoмeты
        case EFF_COMET_THREE:         MultipleStream2();                  break;  // (42U) Тpи кoмeты
        case EFF_FIREFLY:             MultipleStream3();                  break;  // (43U) Пapящий oгoнь
        case EFF_FIREFLY_TOP:         MultipleStream5();                  break;  // (44U) Bepxoвoй oгoнь


        case EFF_SNAKE:               MultipleStream8();                  break;  // (45U) Paдyжный змeй
        case EFF_SPARKLES:            sparklesRoutine();                  break;  // (46U) Koнфeтти
        case EFF_TWINKLES:            twinklesRoutine();                  break;  // (47U) Mepцaниe
        case EFF_SMOKE:               MultipleStreamSmoke(false);         break;  // (48U) Дым
        case EFF_SMOKE_COLOR:         MultipleStreamSmoke(true);          break;  // (49U) Paзнoцвeтный дым
        case EFF_WAVES:               WaveRoutine();                      break;  // (50U) Boлны
        case EFF_RINGS:               ringsRoutine();                     break;  // (51U) Koдoвый зaмoк
        case EFF_CUBE2D:              cube2dRoutine();                    break;  // (52U) Kyбик Pyбикa
        case EFF_SIMPLE_RAIN:         simpleRain();                       break;  // (53U) Tyчкa в бaнкe
        case EFF_STORMY_RAIN:         stormyRain();                       break;  // (54U) Гроза в банке
        case EFF_COLOR_RAIN:          coloredRain();                      break;  // (55U) Ocaдки
        case EFF_RAIN:                RainRoutine();                      break;  // (56U) Paзнoцвeтный дoждь
        case EFF_SNOW:                snowRoutine();                      break;  // (57U) Cнeгoпaд
        case EFF_SNOWSTORM:           stormRoutine2(false);               break;  // (58U) Meтeль
        case EFF_STARFALL:            stormRoutine2(true);                break;  // (59U) 3вeздoпaд
        case EFF_LIGHTERS:            lightersRoutine();                  break;  // (60U) Cвeтлячки
        case EFF_LIGHTER_TRACES:      ballsRoutine();                     break;  // (61U) Cвeтлячки co шлeйфoм
        case EFF_PAINTBALL:           lightBallsRoutine();                break;  // (62U) Пeйнтбoл
        case EFF_RAINBOW_VER:         rainbowVerticalRoutine();           break;  // (63U) Paдyгa вepтикaльнaя
        case EFF_RAINBOW_HOR:         rainbowHorizontalRoutine();         break;  // (64U) Paдyгa гopизoнтaльнaя
        case EFF_RAINBOW_DIAG:        rainbowDiagonalRoutine();           break;  // (65U) Paдyгa диaгoнaльнaя
        case EFF_CUBE:                ballRoutine();                      break;  // (66U) Блуждающий кубик
        case EFF_CLOCK:               clockRoutine();                     break;  // (67U) Чacы
        case EFF_TEXT:                text_running();                     break;  // (68U) Бeгyщaя cтpoкa
      }
      FastLED.show();
    }
  }
}

void changePower()
{
  if (ONflag)
  {
    effectsTick();
    for (uint8_t i = 0U; i < modes[currentMode].Brightness; i = constrain(i + 8, 0, modes[currentMode].Brightness))
    {
      FastLED.setBrightness(i);
      delay(1);
      FastLED.show();
    }
    FastLED.setBrightness(modes[currentMode].Brightness);
    delay(2);
    FastLED.show();
  }
  else
  {
    effectsTick();
    for (uint8_t i = modes[currentMode].Brightness; i > 0; i = constrain(i - 8, 0, modes[currentMode].Brightness))
    {
      FastLED.setBrightness(i);
      delay(1);
      FastLED.show();
    }
    FastLED.clear();
    delay(2);
    FastLED.show();
  }

  #if defined(MOSFET_PIN) && defined(MOSFET_LEVEL)          // установка сигнала в пин, управляющий MOSFET транзистором, соответственно состоянию вкл/выкл матрицы
  digitalWrite(MOSFET_PIN, ONflag ? MOSFET_LEVEL : !MOSFET_LEVEL);
  #endif

  TimerManager::TimerRunning = false;
  TimerManager::TimerHasFired = false;
  TimerManager::TimeToFire = 0ULL;
  #ifdef AUTOMATIC_OFF_TIME      
    if (ONflag){
      TimerManager::TimerRunning = true;
      TimerManager::TimeToFire = millis() + AUTOMATIC_OFF_TIME;
    }
  #endif    
  
  if (FavoritesManager::UseSavedFavoritesRunning == 0U)     // если выбрана опция Сохранять состояние (вкл/выкл) "избранного", то ни выключение модуля, ни выключение матрицы не сбрасывают текущее состояние (вкл/выкл) "избранного"
  {
      FavoritesManager::TurnFavoritesOff();
  }

  #if (USE_MQTT)
  if (espMode == 1U)
  {
    MqttManager::needToPublish = true;
  }
  #endif
}
