#include <stdio.h>
#include <math.h>
#include <string.h>
#pragma warning(disable : 4996 6031)

enum {
    PRENOM_MAX = 31,
    NOM_MAX = 31,
    UE_MAX = 6,
    SEMESTRE_MAX = 6,
    ETUDIANTS_MAX = 100,
    COMMANDE_MAX = 100,
    STATUT_MAX = 20,
    NOTE_MIN = 0,
    NOTE_MAX = 20
};

/*
Structure représentant un étudiant
*/
typedef struct {
    int id;                                 /* Identifiant unique */
    char prenom[PRENOM_MAX];                /* Prénom */
    char nom[NOM_MAX];                      /* Nom */
    float ue[SEMESTRE_MAX][UE_MAX];         /* Notes par semestre et UE */
    int semestreActuel;                     /* Semestre en cours */
    char statut[STATUT_MAX];                /* Statut actuel */
} ETUDIANT;

/*
Vérifie si un étudiant est déjà inscrit
*/
int estDejaInscrit(ETUDIANT etudiants[], int nbEtu, char prenom[], char nom[]) {
    for (int i = 0; i < nbEtu; i++) {
        if (strcmp(etudiants[i].prenom, prenom) == 0 &&
            strcmp(etudiants[i].nom, nom) == 0) {
            return 1;
        }
    }
    return 0;
}

/*
Affiche le cursus d’un étudiant
*/
int Cursus(ETUDIANT etudiants[], int nbEtu) {
    int id;
    scanf("%d", &id);

    if (id < 1 || id > nbEtu) {
        printf("Identifiant incorrect\n");
        return 1;
    }

    printf("%d %s %s\n", id, etudiants[id - 1].prenom, etudiants[id - 1].nom);
    int sActuel = etudiants[id - 1].semestreActuel;

    for (int s = 0; s < sActuel; ++s) {
        printf("S%d - ", s + 1);
        for (int i = 0; i < UE_MAX; ++i) {
            float note = etudiants[id - 1].ue[s][i];
            if (note < 0) {
                printf("* (*) - ");
            }
            else {
                float noteTronquee = floorf(note * 10.f) / 10.f;
                if (noteTronquee >= 10)
                    printf("%.1f (ADM) - ", noteTronquee);
                else
                    printf("%.1f (AJ) - ", noteTronquee);
            }
        }

        if (s == sActuel - 1) {
            if (strcmp(etudiants[id - 1].statut, "demission") == 0)
                printf("demission\n");
            else if (strcmp(etudiants[id - 1].statut, "defaillance") == 0)
                printf("defaillance\n");
            else
                printf("en cours\n");
        }
        else {
            printf("\n");
        }
    }

    return 0;
}

/*
Saisie d’une note pour une UE
*/
float Note(ETUDIANT etudiants[], int nbEtu, int ue, float notes) {
    int id;
    scanf("%d %d %f", &id, &ue, &notes);

    if (id < 1 || id > nbEtu) {
        printf("Identifiant incorrect\n");
        return -1;
    }
    if (ue < 1 || ue > UE_MAX) {
        printf("UE incorrecte\n");
        return -1;
    }
    if (notes < NOTE_MIN || notes > NOTE_MAX) {
        printf("Note incorrecte\n");
        return -1;
    }
    if (strcmp(etudiants[id - 1].statut, "en cours") != 0) {
        printf("Etudiant hors formation\n");
        return -1;
    }

    int s = etudiants[id - 1].semestreActuel - 1;
    etudiants[id - 1].ue[s][ue - 1] = notes;
    printf("Note enregistree\n");
    return notes;
}

/*
Affiche la liste complète des étudiants
*/
void Etudiants(ETUDIANT etudiants[], int nbEtu) {
    for (int i = 0; i < nbEtu; ++i) {
        printf("%d - %s %s - S%d - %s\n",
            etudiants[i].id,
            etudiants[i].prenom,
            etudiants[i].nom,
            etudiants[i].semestreActuel,
            etudiants[i].statut);
    }
}

/*
Enregistre une démission
*/
void Demission(ETUDIANT etudiants[], int nbEtu) {
    int id;
    scanf("%d", &id);

    if (id < 1 || id > nbEtu) {
        printf("Identifiant incorrect\n");
        return;
    }
    if (strcmp(etudiants[id - 1].statut, "en cours") != 0) {
        printf("Etudiant hors formation\n");
        return;
    }

    strcpy(etudiants[id - 1].statut, "demission");
    printf("Demission enregistree\n");
}

/*
Enregistre une défaillance
*/

void Defaillance(ETUDIANT etudiants[], int nbEtu) {
    int id;
    scanf("%d", &id);

    if (id < 1 || id > nbEtu) {
        printf("Identifiant incorrect\n");
        return;
    }
    if (strcmp(etudiants[id - 1].statut, "en cours") != 0) {
        printf("Etudiant hors formation\n");
        return;
    }

    strcpy(etudiants[id - 1].statut, "defaillance");
    printf("Defaillance enregistree\n");
}
/*
afficher les notes et le statut d'un étudiant
*/

void Jury(ETUDIANT etudiants[], int nbEtu) {
    int semestre;
    scanf("%d", &semestre);

    if (semestre < 1 || semestre > SEMESTRE_MAX) {
        printf("Semestre incorrect\n");
        return;
    }

    int manquant = 0;
    for (int i = 0; i < nbEtu; ++i) {
        if (strcmp(etudiants[i].statut, "en cours") == 0 &&
            etudiants[i].semestreActuel == semestre) {
            for (int j = 0; j < UE_MAX; ++j) {
                if (etudiants[i].ue[semestre - 1][j] < 0) {
                    manquant = 1;
                    break;
                }
            }
        }
    }

    if (manquant) {
        printf("Des notes sont manquantes\n");
        return;
    }

    int termine = 0;
    for (int i = 0; i < nbEtu; ++i) {
        if (strcmp(etudiants[i].statut, "en cours") == 0 &&
            etudiants[i].semestreActuel == semestre) {
            if (semestre % 2 == 1) {
                etudiants[i].semestreActuel = semestre + 1;
                for (int j = 0; j < UE_MAX; ++j)
                    etudiants[i].ue[semestre][j] = -1;
                termine++;
            }
        }
    }

    printf("Semestre termine pour %d etudiant(s)\n", termine);
}

/*
Boucle principale
*/
int main() {
    char cde[COMMANDE_MAX];
    ETUDIANT etudiants[ETUDIANTS_MAX];
    int nbEtu = 0;

    do {
        scanf("%s", cde);
        if (strcmp(cde, "INSCRIRE") == 0) {
            char prenom[PRENOM_MAX], nom[NOM_MAX];
            scanf("%s %s", prenom, nom);

            if (estDejaInscrit(etudiants, nbEtu, prenom, nom)) {
                printf("Nom incorrect\n");
            }
            else {
                ETUDIANT* etu = &etudiants[nbEtu];
                strcpy(etu->prenom, prenom);
                strcpy(etu->nom, nom);
                etu->id = nbEtu + 1;
                for (int s = 0; s < SEMESTRE_MAX; ++s)
                    for (int i = 0; i < UE_MAX; ++i)
                        etu->ue[s][i] = -1;
                etu->semestreActuel = 1;
                strcpy(etu->statut, "en cours");
                printf("Inscription enregistree (%d)\n", ++nbEtu);
            }
        }
        else if (strcmp(cde, "NOTE") == 0) {
            int ue = 1;
            float notes = 0;
            Note(etudiants, nbEtu, ue, notes);
        }
        else if (strcmp(cde, "CURSUS") == 0) {
            Cursus(etudiants, nbEtu);
        }
        else if (strcmp(cde, "ETUDIANTS") == 0) {
            Etudiants(etudiants, nbEtu);
        }
        else if (strcmp(cde, "DEMISSION") == 0) {
            Demission(etudiants, nbEtu);
        }
        else if (strcmp(cde, "DEFAILLANCE") == 0) {
            Defaillance(etudiants, nbEtu);
        }
        else if (strcmp(cde, "JURY") == 0) {
            Jury(etudiants, nbEtu);
        }
    } while (strcmp(cde, "EXIT") != 0);
}
