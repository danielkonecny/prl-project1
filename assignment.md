# Implementace algoritmu "Pipeline merge sort"
Pomocí knihovny Open MPI implementujte v jazyce C/C++ algoritmus Pipeline-Merge sort tak, jak byl uveden na přednášce PRL.
Deadline - 9. 4. 2021

## Obecné informace k projektům
* Projekt je opravován automatizovaně, je proto nutné, abyste přesně dodržovali zadání, počet parametrů při spuštění programu, význam těchto parametrů, pojmenování jednotlivých souborů.
* Projekt musí být spustitelný na **Merlinovi**, během vypracovávání projektu **sledujte fórum** konkrétního projektu. Případné dotazy či připomínky také směřujte tam.
* Implementace algoritmů je poměrně triviální, velký důraz je proto kladen na dokumentaci, správnou analýzu složitosti, dále na komunikační protokol, způsob testování. Vedle funkčnosti programu a dokumentace bude hodnocena i přehlednost zdrojového kódu, komentáře, ...
* Komunikační protokol musí být obecný, tedy pro *n* procesů.
* U experimentů je dobré předem si stanovit cíl, za jakým účelem testování vlastně provádíte, pak zhodnotit výsledky. Výsledky testů musí být vhodně reprezentovány, např. grafem. Z desítek hodnot umístěných v tabulce se složitost ověří jen velmi obtížně. V dokumentaci se také očekává přesný popis postupu, který jste zvolili pro měření doby trvání běhu algoritmu.

## Užitečné informace
Knihovna MPI je určena zejména pro praktické použití a autoři nedoporučují spouštět na jednom stroji více procesorů. Jednak poté knihovna běží pomaleji, ale hlavně je každý procesor vytvořen jako samostatný proces v sytému a za běhu mohou být vytvářeny další procesy. Toto může vyústit až v neschopnost spuštění programu, protože mu systém nedovolí vytvořit tolik synovských procesů. Doporučuji proto testovat algoritmy tak, že použijete maximálně 20 procesorů. Pokud však uvidíte hlášku podobnou `merlin.fit.vutbr.cz:24462 [ 15882,0 ,0 ] ORTE_ERROR_LOG: The system limit on number of children a process can have was reached in file base/odls_base_`, nepropadejte panice! Zkušenost ukázala, že stačí nechat merlina chvíli odpočinout (nepřekládat znovu) a poté to zkusit s nižším počtem procesorů.

## Pro zvídavé
Některé limity si můžete zobrazit na svém oblíbeném linuxu pomocí `ulimit -a` a pomocí jednotlivých přepínačů je i měnit. Maximální hodnota je specifikována `ulimit -H -a`. Jsou to tzv. hard limity a pokud nejste administrátor, jejich hodnoty měnit nemůžete. Naše programy budou psány ve stylu SPMD (same program, multiple data), všechny procesory tedy dostanou stejný kód, kde jediným rozpoznávacím znakem daného procesoru je jeho rank. Pokud chcete projekty úspěšně vytvořit, musíte tuto filosofii respektovat. Open MPI neumí měnit po startu počet procesorů, proto je vhodné si vytvořit skript, který toto spočítá dopředu podle použitého algoritmu a počtu vstupních hodnot. Při vypracování projektů tedy můžete počítat s tím, že hodnoty budou při testování zadávány korektně. Pokud budete na merlinovi vytvářet spustitelné skripty, je třeba jim přidat příznak spustitelnosti: `chmod +x file_name`.

## Vstup
Vstupem je posloupnost šestnácti náhodných čísel uložených v souboru.

### Soubor numbers
Soubor *numbers* obsahující čísla velikosti 1 byte, která jdou bez mezery za sebou. Pro příklad vytvoření tohoto souboru prostudujte soubor *test* (sekce [ukázkové zdrojové kódy](https://wis.fit.vutbr.cz/FIT/st/cwk.php.cs?title=1._Projekt&csid=737106&id=14133#uk%C3%A1zkov%C3%A9_zdrojov%C3%A9_k%C3%B3dy)), ve kterém je ukázáno vytvoření takovéto posloupnosti náhodných čísel a její uložení do souboru pomocí utility `dd`. V případě tohoto projektu nastavíte "numbers" napevno na 16. Tato utilita generuje náhodná čísla v rozsahu určeném velikostí bloku. Při bloku 1B jsou hodnoty v rozsahu 0-255. Vygenerovaná čísla jsou pak přesměrována do souboru. Vznikne tedy další soubor s náhodnými znaky jdoucími bez mezery za sebou. Po otevření v libovolném textovém editoru se hodnoty tváří jako náhodné ascii znaky, které by však měly být chápany jako celá čísla. Soubor je v tomto případě chápan jako binární.

## Výstup
Výstup na *stdout* se skládá ze dvou částí:
1. Jednotlivé načtené neseřazené hodnoty v jednom řádku oddělené mezerou (vypsat po načtení prvním procesorem).
2. Jednotlivé seřazené hodnoty oddělené novým řádkem (od nejmenšího po největší).

### Příklad výstupu
```
4 68 1 54 ... 
1
4
54
68
...
```

## Postup
Vytvořte testovací skript *test*, který bude řídit testování. Tento skript bude mít tyto vlastnosti:
* Bude pojmenován *test* nebo *test.sh*.

Skript vytvoří soubor *numbers* s 16ti náhodnými čísly a následně spustí program se správným počtem procesorů. Skript nakonec smaže vytvořenou binárku a soubor *numbers*. Vzhledem ke strojové kontrole výsledků se v odevzdané verzi kódu **nebudou vyskytovat žádné jiné výstupy** než uvedené a ze stejných důvodů je třeba dodržet výše uvedené body týkající se testovacího skriptu. Za nedodržení těchto požadavků budou strhávány body.

## Implementace
Algoritmus implementujte v jazyce C/C++ pomocí knihovny Open MPI.

## Dokumentace
Součástí řešení je dokumentace, která bude o rozsahu **maximálně 3 strany** (rozumné a odůvodněné překročení limitu stran není důvod k bodové srážce) funkčního textu.

Do dokumentace **nedávejte**:
* Úvodní stranu, obsah, popis zadání.

V dokumentaci **popište**:
* Rozbor a analýzu algoritmu, odvoďte jeho teoretickou složitost (časovou a prostorovou složitost, celkovou cenu). Uvedené vzorce slovně popište, včetně jejich proměnných.
* Implementaci.
* Komunikační protokol, jak si "procesy" zasílají zprávy. Pro vizualizaci použijte [sekvenční diagram](http://en.wikipedia.org/wiki/Sequence_diagram). Nezapomeňte, že protokol musí být obecný, tedy pro n procesů.
* Závěr, ve kterém zhodnotíte dosažené výsledky.
* Pozor, hodnotí se i to, jak dokumentace působí! (Zejména vzhled, správná čeština/slovenština/angličtina.)

Knihovna Open MPI je navržena na praktické použití, proto neobsahuje žádné implicitní metody pro měření složitosti algoritmů. Je tedy třeba vymyslet nějaký explicitní způsob jejího měření a ten pak co nejdetailněji popsat v dokumentaci!

## Odevzdání
Do wisu se odevzdává jeden archiv xlogin00.{tar|tgz|zip}, který bude velký do 1MB, a který obsahuje:
* zdrojový kód- pms.{c|cpp},
* hlavička- pms.h (pokud ji využijete),
* testovací shellový skript- {test|test.sh},
* dokumentaci- xlogin00.pdf,
* nic jiného...

Platí, že kvalita každé z části vzhledem k požadavkům má vliv na bodové ohodnocení . Počítejte s tím, že veškerá uvedená jména souborů jsou *case sensitive*.

## Hodnocení
Způsob možné reklamace projektu bude zveřejněn později.
