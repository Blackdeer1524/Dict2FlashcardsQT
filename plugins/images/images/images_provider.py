import json
import os
import re
from typing import Any, Literal, TypedDict, Union

import bs4
import requests

PLUGIN_NAME = os.path.split(os.path.dirname(__file__))[-1]


def get(word: str):
    link = f"https://www.google.com/search?tbm=isch&q={word}"
    user_agent = (
        "Mozilla/5.0 (X11; Linux x86_64; rv:109.0) Gecko/20100101 Firefox/112.0"
    )
    headers = {"User-Agent": user_agent}
    try:
        r = requests.get(link, headers=headers)
        r.raise_for_status()
    except requests.RequestException:
        return [], f"[{PLUGIN_NAME}]: Couldn't get a web page!"

    html = r.text
    soup = bs4.BeautifulSoup(r.text, "html.parser")
    rg_meta = soup.find_all("div", {"class": "rg_meta"})
    metadata = [json.loads(e.text) for e in rg_meta]
    results = [d["ou"] for d in metadata]

    batch_size = yield
    if not results:
        regex = re.escape("AF_initDataCallback({")
        regex += r"[^<]*?data:[^<]*?" + r"(\[[^<]+\])"

        for txt in re.findall(regex, html):
            data = json.loads(txt)

            try:
                for d in data[31][0][12][2]:
                    try:
                        results.append(d[1][3][0])
                        if not len(results) % batch_size:
                            batch_size = (
                                yield {
                                    "web": [(link, "") for link in results],
                                    "local": [],
                                },
                                "",
                            )
                            results = []
                    except Exception as exception:
                        pass
            except Exception as exception:
                try:
                    for d in data[56][1][0][0][1][0]:
                        try:
                            results.append(d[0][0]["444383007"][1][3][0])
                            if not len(results) % batch_size:
                                batch_size = (
                                    yield {
                                        "web": [(link, "") for link in results],
                                        "local": [],
                                    },
                                    "",
                                )
                                results = []
                        except Exception as exception:
                            pass
                except Exception as exception:
                    pass
    return results, ""


def load():
    return


def unload():
    return


ConfigKeys = Literal["timeout"]
TIMEOUT = "timeout"


class ConfigFieldInfo(TypedDict):
    docs: str
    type: str


ConfigDescription = dict[
    ConfigKeys, Union[ConfigFieldInfo, "ConfigDescription"]
]


def get_config_description() -> ConfigDescription:
    return {
        TIMEOUT: {"docs": "Request timeout", "type": "number"},
    }


def get_default_config() -> dict[ConfigKeys, Any]:
    return {TIMEOUT: 3}


class ErrorSummary(TypedDict):
    error_type: Literal[
        "invalid_type", "invalid_value", "empty", "unknown_field"
    ]
    description: str


SetConfigError = dict[str, Union[ErrorSummary, "SetConfigError"]]


def validate_config(new_config: dict) -> SetConfigError:
    res: SetConfigError = {}

    if (new_timeout := new_config.get(TIMEOUT)) is not None:
        if type(new_timeout) not in [float, int]:
            res[TIMEOUT] = {
                "error_type": "invalid_type",
                "description": f"`{TIMEOUT}` is expected to be a number. Got `{new_timeout}`",
            }
        elif new_timeout < 0:
            res[TIMEOUT] = {
                "error_type": "invalid_value",
                "description": f"`{TIMEOUT}` is expected to be non negative",
            }

    else:
        res[TIMEOUT] = {
            "error_type": "empty",
            "description": "`timeout` was left empty",
        }

    conf_keys = [TIMEOUT]
    for key in new_config:
        if key in conf_keys:
            continue
        res[key] = {
            "error_type": "unknown_field",
            "description": f"Unknown field: `{key}`",
        }

    return res


if __name__ == "__main__":
    a = get("word")
    next(a)
    while True:
        res = a.send(1)
        print(res)
