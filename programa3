#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MaximoNumeroDeSensores 50
#define MaximoTamanhoID 32
#define MaximoTamanhoTipo 16
#define MaximoTamanhoValor 32

typedef struct {
    char id[MaximoTamanhoID];
    char tipo[MaximoTamanhoTipo];
} DadosDoSensor;

long StringParaTimestamp(const char* DataHoraString) {
    struct tm t = {0};
    if (!strptime(DataHoraString, "%Y-%m-%d %H:%M:%S", &t)) return -1;
    return mktime(&t);
}

void GeraValorAleatorio(char* val, const char* tipo) {
    if (strcmp(tipo, "int") == 0)
        sprintf(val, "%d", rand() % 10000);
    else if (strcmp(tipo, "float") == 0)
        sprintf(val, "%.2f", ((float)rand() / RAND_MAX) * 100.0);
    else if (strcmp(tipo, "bool") == 0)
        strcpy(val, rand() % 2 ? "true" : "false");
    else if (strcmp(tipo, "string") == 0) {
        int len = rand() % 16 + 1;
        for (int i = 0; i < len; i++)
            val[i] = 'A' + rand() % 26;
        val[len] = '\0';
    } else
        strcpy(val, "NA");
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        printf("Uso correto: %s <DataInicio> <DataFim> <ArquivoSensores> <ArquivoSaida>\n", argv[0]);
        return 1;
    }

    srand(time(NULL));
    long inicio = StringParaTimestamp(argv[1]);
    long fim = StringParaTimestamp(argv[2]);
    if (inicio == -1 || fim == -1 || fim <= inicio) {
        printf("As datas fornecidas são inválidas!\n");
        return 1;
    }

    FILE* fsens = fopen(argv[3], "r");
    if (!fsens) {
        printf("Falha ao abrir o arquivo de sensores!\n");
        return 1;
    }

    DadosDoSensor sensores[MaximoNumeroDeSensores];
    int total = 0;
    while (fscanf(fsens, "%s %s", sensores[total].id, sensores[total].tipo) == 2 && total < MaximoNumeroDeSensores) {
        total++;
    }
    fclose(fsens);

    FILE* fsaida = fopen(argv[4], "w");
    if (!fsaida) {
        printf("Não foi possível abrir o arquivo de saída\n");
        return 1;
    }

    for (int i = 0; i < total; i++) {
        for (int j = 0; j < 2000; j++) {
            long ts = inicio + rand() % (fim - inicio + 1);
            char valor[MaximoTamanhoValor];
            GeraValorAleatorio(valor, sensores[i].tipo);
            fprintf(fsaida, "%ld %s %s\n", ts, sensores[i].id, valor);
        }
    }

    fclose(fsaida);
    printf("Arquivo de teste gerado com sucesso: %s\n", argv[4]);
    return 0;
}
