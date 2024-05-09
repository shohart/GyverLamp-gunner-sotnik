// Если вы хотите добавить эффекты или сделать им копии для демонстрации на разных настройках, нужно делать это в 3 местах (помимо добавления программы самого эффекта ):
// 0. в файле effects.ino - добавляется программный код самого эффекта.
// 2. в файле Constants.h - придумываются названия "EFF_......" и задаются порядковые номера эффектам. В конце указывается общее количество MODE_AMOUNT.
//    Не изменяйте порядковые номера у эффектов с 0U по 15U. У них иное управление скоростью.
// 3. там же в файле Constants.h ниже - задаётся Реестр эффектов для передачи в приложение. Он живёт отдельно. 
//    Если у вас приложение не поддерживает запрос реестра у лампы, реестр можно не менять.
// 4. в файле effectTicker.ino - подключается процедура вызова эффекта на соответствующий ей "EFF_......"
//    Можно подключать один и тот же эффект под разными номерами. Например EFF_FIRE (1U), EFF_FIRE2 (26U), EFF_FIRE3 (27U). Будет три огня разных цветов.

uint32_t effTimer;

void effectsTick()
{
  if (!dawnFlag)
  {
    if (ONflag && (millis() - effTimer >= ((currentMode > 19U ) ? 256U - modes[currentMode].Speed : (currentMode < 11U ) ? 50 : 15))) // у эффектов до 19U вкюччительно (все перед Огнём) бегунок Скорость не регулирует задержку
    {
      effTimer = millis();
      switch (currentMode)
      {
        case EFF_WHITE_COLOR:         whiteColorStripeRoutine();          break;  // ( 0U) Белый свет
        case EFF_COLOR:               colorRoutine();                     break;  // ( 1U) Цвет
        case EFF_MADNESS:             madnessNoiseRoutine();              break;  // ( 2U) Безумие
        case EFF_CLOUDS:              cloudsNoiseRoutine();               break;  // ( 3U) Облака
        case EFF_LAVA:                lavaNoiseRoutine();                 break;  // ( 4U) Лава
        case EFF_PLASMA:              plasmaNoiseRoutine();               break;  // ( 5U) Плазма
        case EFF_RAINBOW:             rainbowNoiseRoutine();              break;  // ( 6U) Радуга 3D
        case EFF_RAINBOW_STRIPE:      rainbowStripeNoiseRoutine();        break;  // ( 7U) Павлин
        case EFF_ZEBRA:               zebraNoiseRoutine();                break;  // ( 8U) Зебра
        case EFF_FOREST:              forestNoiseRoutine();               break;  // ( 9U) Лес
        case EFF_OCEAN:               oceanNoiseRoutine();                break;  // (10U) Океан
        case EFF_BBALLS:              BBallsRoutine(true);                break;  // (11U) Мячики
        case EFF_BBALLS_WHITE:        BBallsRoutine(false);               break;  // (12U) Белые мячики
        case EFF_BBALLS_TRACES:       BBallsRoutine(true);                break;  // (13U) Мячики со шлейфом
        case EFF_SPIRO:               spiroRoutine();                     break;  // (14U) Спирали
        case EFF_PRISMATA:            PrismataRoutine();                  break;  // (15U) Призмата
        case EFF_FLOCK:               flockRoutine(false);                break;  // (16U) Стая
        case EFF_FLOCK_N_PR:          flockRoutine(true);                 break;  // (17U) Стая и хищник
        case EFF_SINUSOID3:           Sinusoid3Routine();                 break;  // (18U) Синусоид
        case EFF_METABALLS:           MetaBallsRoutine();                 break;  // (19U) Метаболз


        case EFF_FIRE:                fireRoutine(true);                  break;  // (20U) Огонь
        case EFF_FIRE_WHITTE:         fireRoutine(true);                  break;  // (21U) Белый огонь
        case EFF_FIRE_BLUE:           fireRoutine(true);                  break;  // (22U) Голубой огонь
        case EFF_FIRE_GREEN:          fireRoutine(true);                  break;  // (23U) Зелёный огонь
        case EFF_MATRIX:              matrixRoutine();                    break;  // (24U) Матрица
        case EFF_WATERFALL:           fire2012WithPalette();              break;  // (25U) Водопад
        case EFF_WATERFALL_WHITE:     fire2012WithPalette();              break;  // (26U) Белый водопад
        case EFF_WATERFALL_4IN1:      fire2012WithPalette4in1();          break;  // (27U) Водопад 4 в 1
        case EFF_POOL:                poolRoutine();                      break;  // (28U) Бассейн
        case EFF_PULSE_SLOW:          pulseRoutine(2U);                   break;  // (29U) Медленный пульс
        case EFF_PULSE_FAST:          pulseRoutine(1U);                   break;  // (30U) Быстрый пульс
        case EFF_PULSE_RAINBOW:       pulseRoutine(4U);                   break;  // (31U) Радужный пульс
        case EFF_PULSE_WHITE:         pulseRoutine(8U);                   break;  // (32U) Белый пульс
        case EFF_COMET:               RainbowCometRoutine();              break;  // (33U) Комета
        case EFF_COMET_WHITE:         ColorCometRoutine();                break;  // (34U) Белая комета
        case EFF_COMET_COLOR:         ColorCometRoutine();                break;  // (35U) Одноцветная комета
        case EFF_COMET_PULSING:       MultipleStream4();                  break;  // (36U) Пульсирующая комета
        case EFF_COMET_TWO:           MultipleStream();                   break;  // (37U) Две кометы
        case EFF_COMET_THREE:         MultipleStream2();                  break;  // (38U) Три кометы


        case EFF_FIREFLY:             MultipleStream3();                  break;  // (39U) Парящий огонь
        case EFF_FIREFLY_TOP:         MultipleStream5();                  break;  // (40U) Верховой огонь
        case EFF_SNAKE:               MultipleStream8();                  break;  // (41U) Радужный змей
        case EFF_SPARKLES:            sparklesRoutine();                  break;  // (42U) Конфетти
        case EFF_RAINBOW_VER:         rainbowVerticalRoutine();           break;  // (43U) Радуга вертикальная
        case EFF_RAINBOW_HOR:         rainbowHorizontalRoutine();         break;  // (44U) Радуга горизонтальная
        case EFF_RAINBOW_DIAG:        rainbowDiagonalRoutine();           break;  // (45U) Радуга диагональная
        case EFF_WAVES:               WaveRoutine();                      break;  // (46U) Волны
        case EFF_SNOW:                snowRoutine();                      break;  // (47U) Снегопад
        case EFF_RAIN:                RainRoutine();                      break;  // (48U) Цветной дождь
        case EFF_SNOWSTORM:           snowStormRoutine();                 break;  // (49U) Метель
        case EFF_STARFALL:            starfallRoutine();                  break;  // (50U) Звездопад
        case EFF_LIGHTERS:            lightersRoutine();                  break;  // (51U) Светлячки
        case EFF_LIGHTER_TRACES:      ballsRoutine();                     break;  // (52U) Светлячки со шлейфом
        case EFF_PAINTBALL:           lightBallsRoutine();                break;  // (53U) Пейнтбол
        case EFF_CUBE:                ballRoutine();                      break;  // (54U) Блуждающий кубик
        case EFF_COLORS:              colorsRoutine();                    break;  // (55U) Смена цвета
        case EFF_TEXT:                text_running();                     break;  // (56U) Бегущая строка
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
