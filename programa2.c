#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MaximoTamanhoValor 64

typedef struct {
    long timestamp;
    char valor[MaximoTamanhoValor];
} DadosDoSensor;

long ConverterDataParaTimestamp(const char* str) {
    struct tm t = {0};
    int ano, mes, dia, hora, minuto, segundo;

    if (sscanf(str, "%d-%d-%d %d:%d:%d", &ano, &mes, &dia, &hora, &minuto, &segundo) != 6)
        return -1;
    t.tm_year = ano - 1900;
    t.tm_mon = mes - 1;
    t.tm_mday = dia;
    t.tm_hour = hora;
    t.tm_min = minuto;
    t.tm_sec = segundo;

    return mktime(&t);
}

long ValorAbsoluto(long x) {
    return x < 0 ? -x : x;
}

int BuscarPorTimestamp(DadosDoSensor* Sensor, int total, long TimestampAlvo) {
    int inicio = 0, fim = total - 1;
    int MaisProximo = 0;
    long MenorDiferenca = ValorAbsoluto(Sensor[0].timestamp - TimestampAlvo);

    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        long diferenca = ValorAbsoluto(Sensor[meio].timestamp - TimestampAlvo);

        if (diferenca < MenorDiferenca) {
            MenorDiferenca = diferenca;
            MaisProximo = meio;
        }

        if (Sensor[meio].timestamp < TimestampAlvo)
            inicio = meio + 1;
        else
            fim = meio - 1;
    }
    return MaisProximo;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Uso: %s <Nome do Sensor> <\"YYYY-MM-DD HH:MM:SS\">\n", argv[0]);
        return 1;
    }

    long TimestampAlvo = ConverterDataParaTimestamp(argv[2]);
    if (TimestampAlvo == -1) {
        printf("Formato de data invalido!\n");
        return 1;
    }

    char arquivo[100];
    snprintf(arquivo, sizeof(arquivo), "%s.txt", argv[1]);
    FILE* f = fopen(arquivo, "r");
    if (!f) {
        printf("Nao foi possível abrir o arquivo %s\n", arquivo);
        return 1;
    }

    DadosDoSensor Sensor[10000];
    int contador = 0;
    while (fscanf(f, "%ld %s", &Sensor[contador].timestamp, Sensor[contador].valor) == 2) {
        contador++;
    }
    fclose(f);

    if (contador == 0) {
        printf("O arquivo esta vazio!\n");
        return 1;
    }

    int indice = BuscarPorTimestamp(Sensor, contador, TimestampAlvo);
    printf("Leitura mais proxima encontrada:\n%ld %s\n", Sensor[indice].timestamp, Sensor[indice].valor);
    
    return 0;
}
