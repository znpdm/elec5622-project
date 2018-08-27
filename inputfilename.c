#include <sys/types.h>
#include <stdio.h>
#include <db.h>
#include <stdlib.h>
#include <string.h>

#define	DATABASE "access.db"

int main()
{
	DB *dbp;
	DBT key, data,DOB;
	int ret, t_ret;

	if ((ret = db_create(&dbp, NULL, 0)) != 0) {
		fprintf(stderr, "db_create: %s\n", db_strerror(ret));
		exit (1);
	}
	if ((ret = dbp->open(dbp,
	    NULL, DATABASE, NULL, DB_BTREE, DB_CREATE, 0664)) != 0) {
		dbp->err(dbp, ret, "%s", DATABASE);
		goto err;
	}

	memset(&key, 0, sizeof(key));
	memset(&data, 0, sizeof(data));
	memset(&DOB,0,sizeof(DOB));

	key.data = "Chen";
	key.size = sizeof("Chen");
	data.data = "Jianhui";
	data.size = sizeof("Jianhui");
	DOB.data = "19/10";
	DOB.size = sizeof("19/10");

	if ((ret = dbp->put(dbp, NULL, &key, &data, 0)) == 0)
		printf("db: %s: key stored.\n", (char *)key.data);
	else {
		dbp->err(dbp, ret, "DB->put");
		goto err;
	}
	if ((ret = dbp->get(dbp, NULL, &key, &data, 0)) == 0)
		printf("db: %s: key retrieved: data was %s.\n",
		    (char *)key.data, (char *)data.data);
	else {
		dbp->err(dbp, ret, "DB->get");
		goto err;
	}
	if ((ret = dbp->get(dbp, NULL, &key, &data, 0)) == 0)
		printf("db: %s: key retrieved: DOB was %s.\n",
		    (char *)key.data, (char *)DOB.data);
	else {
		dbp->err(dbp, ret, "DB->get");
		goto err;
	}

	if ((ret = dbp->del(dbp, NULL, &key, 0)) == 0)
		printf("db: %s: key was deleted.\n", (char *)key.data);
	else {
		dbp->err(dbp, ret, "DB->del");
		goto err;
	}

	if ((ret = dbp->get(dbp, NULL, &key, &data, 0)) == 0)
		printf("db: %s: key retrieved: data was %s.\n",
		    (char *)key.data, (char *)data.data);
	else
		dbp->err(dbp, ret, "DB->get");
err:	if ((t_ret = dbp->close(dbp, 0)) != 0 && ret == 0)
		ret = t_ret; 

	exit(ret);
}

